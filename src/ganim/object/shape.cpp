#include "shape.hpp"

#include <ranges>

#include "ganim/gl/gl.hpp"
#include "ganim/ga/exp.hpp"
#include "ganim/rate_functions.hpp"
#include "shaders.hpp"

using namespace ganim;

Shape::Shape(
    std::vector<Vertex> vertices,
    std::vector<unsigned> indices
)
{
    set_vertices(std::move(vertices), std::move(indices));
}

void Shape::set_vertices(
    std::vector<Vertex> vertices,
    std::vector<unsigned> indices
)
{
    M_vertices = std::move(vertices);
    M_indices = std::move(indices);
    if (!M_vertices.empty()) {
        reset_draw_fractions();
        M_do_shading = false;
        for (auto& v : M_vertices) {
            if (v.z != M_vertices[0].z) {
                M_do_shading = true;
                break;
            }
        }
    }
    M_opengl_valid = false;
    M_changed_after_construction = true;
}

void Shape::reset_draw_fractions()
{
    auto ts = M_vertices
        | std::views::transform([](const auto& v) {return v.t;});
    M_min_draw_fraction = *std::ranges::min_element(ts);
    M_max_draw_fraction = *std::ranges::max_element(ts);
    auto dif = M_max_draw_fraction - M_min_draw_fraction;
    M_min_draw_fraction -= dif / 50;
}

Shape::Shape(const Shape& other)
    : SingleObject(other),
    M_vertices(other.M_vertices),
    M_indices(other.M_indices),
    M_min_draw_fraction(other.M_min_draw_fraction),
    M_max_draw_fraction(other.M_max_draw_fraction),
    M_opengl_valid(false), // This will make the OpenGL things get remade
    M_do_shading(other.M_do_shading)
{}

void Shape::draw(const Camera& camera)
{
    if (M_vertices.empty()) return;
    if (!M_opengl_valid) {
        glBindVertexArray(M_vertex_array);
        glBindBuffer(GL_ARRAY_BUFFER, M_vertex_buffer);
        buffer_vertices();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_element_buffer);
        buffer_indices();
        glBindVertexArray(0);
        M_opengl_valid = true;
    }
    auto& shader = ganim::get_shader(get_shader_flags());
    glUseProgram(shader);
    set_subclass_uniforms(shader);
    if (auto buffer = peeling_depth_buffer()) {
        glUniform1i(shader.get_uniform("layer_depth_buffer"), 15);
        glActiveTexture(GL_TEXTURE15);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, *buffer);
    }
    glUniform2f(shader.get_uniform("camera_scale"),
                camera.get_x_scale(), camera.get_y_scale());
    auto view = ~camera.get_rotor();
    shader.set_rotor_uniform("view", view);
    auto model = get_rotor();
    if (is_fixed_orientation()) {
        using namespace pga3;
        auto view_euclidean =
            view.blade_project<e>() +
            view.blade_project<e12>() * e12 +
            view.blade_project<e13>() * e13 +
            view.blade_project<e23>() * e23;
        model = ~view_euclidean * model;
    }
    shader.set_rotor_uniform("model", model);
    auto color = get_color();
    color.a *= get_opacity();
    glUniform4f(shader.get_uniform("object_color"),
            color.r / 255.0, color.g / 255.0,
            color.b / 255.0, color.a / 255.0);
    glUniform1f(shader.get_uniform("scale"), get_scale());
    glUniform1f(shader.get_uniform("depth_z"), get_depth_z());
    if (is_creating()) {
        auto actual_draw_fraction = M_min_draw_fraction
            + (M_max_draw_fraction - M_min_draw_fraction) * get_draw_fraction();
        glUniform1f(shader.get_uniform("this_t"), actual_draw_fraction);
    }
    else if (auto noise = noise_creating()) {
        auto actual_noise = noise * (M_max_draw_fraction - M_min_draw_fraction);
        auto actual_min = M_min_draw_fraction - actual_noise;
        auto actual_max = M_max_draw_fraction + actual_noise;
        auto actual_draw_fraction = actual_min
            + (actual_max - actual_min) * get_draw_fraction();
        glUniform1f(shader.get_uniform("this_t"), actual_draw_fraction);
        glUniform1f(shader.get_uniform("noise_scale"), actual_noise);
    }
    if (M_pixelate_size) {
        glUniform1i(shader.get_uniform("pixel_size"), M_pixelate_size);
    }
    if (get_squish_amount() != 1.0) {
        glUniform1f(shader.get_uniform("squish_amount"), get_squish_amount());
        shader.set_plane_uniform("squish_axis", get_squish_axis());
    }
    glBindVertexArray(M_vertex_array);
    glDrawElements(GL_TRIANGLES, M_indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Shape::interpolate(
    const Animatable& start,
    const Animatable& end,
    double t
)
{
    SingleObject::interpolate(start, end, t);
    auto start2 = dynamic_cast<const Shape*>(&start);
    auto end2 = dynamic_cast<const Shape*>(&end);
    if (!start2 or !end2) return;

    auto error = []{
        throw std::invalid_argument("At least for now, you can only interpolate"
                " between two Shapes that have the same number of vertices and "
                "indices.");
    };
    if (start2->M_indices.size() != end2->M_indices.size()) error();
    if (start2->M_vertices.size() != end2->M_vertices.size()) error();
    if (M_indices.size() != end2->M_indices.size()) error();
    if (M_vertices.size() != end2->M_vertices.size()) error();
    if (start2->M_changed_after_construction or
            end2->M_changed_after_construction) {
        for (int i = 0; i < ssize(M_vertices); ++i) {
            auto& v1 = start2->M_vertices[i];
            auto& v2 = M_vertices[i];
            auto& v3 = end2->M_vertices[i];
            v2.x = (1 - t)*v1.x + t*v3.x;
            v2.y = (1 - t)*v1.y + t*v3.y;
            v2.z = (1 - t)*v1.z + t*v3.z;
            v2.t = (1 - t)*v1.t + t*v3.t;
            v2.r = (1 - t)*v1.r + t*v3.r;
            v2.g = (1 - t)*v1.g + t*v3.g;
            v2.b = (1 - t)*v1.b + t*v3.b;
            v2.a = (1 - t)*v1.a + t*v3.a;
        }
        M_opengl_valid = false;
        M_changed_after_construction = true;
    }
    if (start2->M_do_shading) M_do_shading = true;
}

ObjectPtr<Shape> Shape::copy() const
{
    return ObjectPtr<Shape>::from_new(copy_impl());
}

Shape* Shape::copy_impl() const
{
    return new Shape(*this);
}

void Shape::pixelate(int pixel_size)
{
    M_pixelate_size = pixel_size;
}

void Shape::buffer_vertices()
{
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*M_vertices.size(),
                 M_vertices.empty() ? nullptr : M_vertices.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(4*sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Shape::buffer_indices()
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*M_indices.size(),
                 M_indices.data(), GL_STATIC_DRAW);
}

ShaderFeature Shape::get_shader_flags()
{
    using enum ShaderFeature;
    auto flags = Time | VertexColors;
    if (peeling_depth_buffer()) flags |= DepthPeeling;
    if (is_creating()) flags |= Create;
    else if (noise_creating()) flags |= NoiseCreate;
    if (M_do_shading) flags |= FaceShading;
    if (M_pixelate_size) flags |= Pixelate;
    if (get_squish_amount() != 1.0) flags |= Squish;
    return flags;
}

Box Shape::get_original_true_bounding_box() const
{
    if (M_vertices.empty()) return Box();
    // TODO: Use caching to be more efficient
    using namespace vga3;
    auto convert_point = [&](const Vertex& p) {
        return p.x*e1 + p.y*e2 + p.z*e3;
    };
    auto transformed_points = M_vertices | std::views::transform(convert_point);
    auto xs = transformed_points
        | std::views::transform(&vga3::Vec::blade_project<e1>);
    auto ys = transformed_points
        | std::views::transform(&vga3::Vec::blade_project<e2>);
    auto zs = transformed_points
        | std::views::transform(&vga3::Vec::blade_project<e3>);
    return Box(
        vga3::Vec{
            *std::ranges::min_element(xs),
            *std::ranges::min_element(ys),
            *std::ranges::min_element(zs)
        },
        vga3::Vec{
            *std::ranges::max_element(xs),
            *std::ranges::max_element(ys),
            *std::ranges::max_element(zs)
        }
    );
}

ObjectPtr<Shape> ganim::make_cube(double width)
{
    auto r = float(width/2);
    auto points = std::vector<Shape::Vertex>{
        {-r, -r, -r, 0},
        {+r, -r, -r, 1},
        {-r, +r, -r, 2},
        {+r, +r, -r, 3},
        {-r, -r, +r, 4},
        {+r, -r, +r, 5},
        {-r, +r, +r, 6},
        {+r, +r, +r, 7},
    };
    auto indices = std::vector<unsigned>{
        0, 2, 1,
        1, 2, 3,
        0, 5, 4,
        0, 1, 5,
        1, 7, 5,
        1, 3, 7,
        2, 6, 7,
        2, 7, 3,
        0, 4, 6,
        0, 6, 2,
        4, 5, 6,
        5, 7, 6
    };
    return make_shape(std::move(points), std::move(indices));
}

ObjectPtr<Shape> ganim::make_sphere(
    double radius,
    int horizontal_precision,
    int vertical_precision
)
{
    using namespace vga3;
    auto points = std::vector<Vec>();
    auto indices = std::vector<unsigned>();
    points.reserve(horizontal_precision * vertical_precision);
    indices.reserve(horizontal_precision * vertical_precision * 3);

    points.push_back(radius*e3);
    for (int i = 1; i < vertical_precision; ++i) {
        auto starting_index = ssize(points);
        auto start_point = e3 * ga_exp(e31 * τ/2 * i / vertical_precision);
        auto extra_rot = i % 2 == 0 ? 0 : 0.5;
        for (int j = 0; j < horizontal_precision; ++j) {
            auto r = ga_exp(e12 * τ/2 * (j + extra_rot) / horizontal_precision);
            auto this_point = (~r * start_point * r).grade_project<1>();
            points.push_back(radius*this_point);
        }
        if (i == 1) {
            for (int j = 0; j < horizontal_precision; ++j) {
                indices.push_back(0);
                indices.push_back(j+1);
                indices.push_back((j+1)%horizontal_precision + 1);
            }
        }
        else {
            auto p = starting_index - horizontal_precision; // Previous
            auto c = starting_index; // Current
            for (int j = 0; j < horizontal_precision; ++j) {
                auto j0 = j;
                auto j1 = (j+1)%horizontal_precision;
                // This row is to the left of the previous row
                if (i % 2 == 0) {
                    indices.push_back(p+j0);
                    indices.push_back(c+j0);
                    indices.push_back(c+j1);

                    indices.push_back(p+j0);
                    indices.push_back(c+j1);
                    indices.push_back(p+j1);
                }
                // This row is to the right of the previous row
                else {
                    indices.push_back(p+j0);
                    indices.push_back(c+j0);
                    indices.push_back(p+j1);

                    indices.push_back(p+j1);
                    indices.push_back(c+j0);
                    indices.push_back(c+j1);
                }
            }
        }
    }
    auto c = ssize(points) - horizontal_precision;
    auto f = ssize(points)-1;
    points.push_back(-radius*e3);
    for (int j = 0; j < horizontal_precision; ++j) {
        indices.push_back(j+c);
        indices.push_back(f);
        indices.push_back((j+1)%horizontal_precision + c);
    }

    auto vertices = std::vector<Shape::Vertex>();
    vertices.reserve(points.size());
    int t = 0;
    for (auto& p : points) {
        vertices.push_back({
            static_cast<float>(p.blade_project<e1>()),
            static_cast<float>(p.blade_project<e2>()),
            static_cast<float>(p.blade_project<e3>()),
            static_cast<float>(++t)
        });
    }
    return make_shape(std::move(vertices), std::move(indices));
}
