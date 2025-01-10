#include "shape.hpp"

#include <ranges>

#include "ganim/gl/gl.hpp"

#include "shaders.hpp"

#include "ganim/rate_functions.hpp"

#include <iostream>

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
    shader.set_rotor_uniform("view", ~camera.get_rotor());
    shader.set_rotor_uniform("model", get_rotor());
    glUniform4f(shader.get_uniform("object_color"),
            get_color().r / 255.0, get_color().g / 255.0,
            get_color().b / 255.0, get_color().a / 255.0);
    glUniform1f(shader.get_uniform("scale"), get_scale());
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
}

ObjectPtr<Shape> Shape::polymorphic_copy() const
{
    return ObjectPtr<Shape>::from_new(polymorphic_copy_impl());
}

Shape* Shape::polymorphic_copy_impl() const
{
    return new Shape(*this);
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
