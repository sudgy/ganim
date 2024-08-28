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
        auto ts = M_vertices
            | std::views::transform([](const auto& v) {return v.t;});
        M_min_draw_fraction = *std::ranges::min_element(ts);
        M_max_draw_fraction = *std::ranges::max_element(ts);
    }
    M_valid = false;
}

void Shape::draw(const Camera& camera)
{
    if (M_vertices.empty()) return;
    if (!M_valid) {
        glBindVertexArray(M_vertex_array);
        glBindBuffer(GL_ARRAY_BUFFER, M_vertex_buffer);
        buffer_vertices();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_element_buffer);
        buffer_indices();
        glBindVertexArray(0);
        M_valid = true;
    }
    auto& shader = *get_shader();
    glUseProgram(shader);
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

gl::Shader* Shape::get_shader()
{
    if (is_creating()) {
        return &ganim::get_shader({
            &basic_shader_parts(),
            &create_shader_parts()
        });
    }
    else if (noise_creating()) {
        return &ganim::get_shader({
            &basic_shader_parts(),
            &noise_create_shader_parts()
        });
    }
    else {
        return &ganim::get_shader({
            &basic_shader_parts()
        });
    }
}

Box Shape::get_true_bounding_box() const
{
    if (M_vertices.empty()) return Box();
    // TODO: Use caching to be more efficient
    using namespace vga3;
    auto rotor = get_rotor();
    auto convert_point = [&](const Vertex& p) {
        auto p_vec = p.x*e1 + p.y*e2 + p.z*e3;
        auto new_p = ~rotor * vga3_to_pga3(get_scale()*p_vec) * rotor;
        return pga3_to_vga3(new_p.grade_project<3>());
    };
    auto transformed_points = M_vertices | std::views::transform(convert_point);
    auto xs = transformed_points
        | std::views::transform(&vga3::Vector::blade_project<e1>);
    auto ys = transformed_points
        | std::views::transform(&vga3::Vector::blade_project<e2>);
    auto zs = transformed_points
        | std::views::transform(&vga3::Vector::blade_project<e3>);
    return Box(
        {
            *std::ranges::min_element(xs),
            *std::ranges::min_element(ys),
            *std::ranges::min_element(zs)
        },
        {
            *std::ranges::max_element(xs),
            *std::ranges::max_element(ys),
            *std::ranges::max_element(zs)
        }
    );
}
