#include "shape.hpp"

#include <ranges>

#include "ganim/gl/gl.hpp"

#include "shaders.hpp"

#include "ganim/rate_functions.hpp"

using namespace ganim;

Shape::Shape(
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned> indices
) : M_index_size(indices.size())
{
    auto ts = vertices | std::views::transform([](const auto& v) {return v.t;});
    M_min_draw_fraction = *std::ranges::min_element(ts);
    M_max_draw_fraction = *std::ranges::max_element(ts);
    glBindVertexArray(M_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, M_vertex_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_element_buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(),
                 &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(4*sizeof(float)));
    glEnableVertexAttribArray(2);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*indices.size(),
                 indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Shape::draw()
{
    auto& shader = shape_shader();
    glUseProgram(shader);
    shader.set_rotor_uniform("model", get_rotor());
    glUniform4f(shader.get_uniform("object_color"),
            get_color().r / 255.0, get_color().g / 255.0,
            get_color().b / 255.0, get_color().a / 255.0);
    glUniform1f(shader.get_uniform("scale"), get_scale());
    auto actual_draw_fraction = M_min_draw_fraction
        + (M_max_draw_fraction - M_min_draw_fraction) * get_draw_fraction();
    glUniform1f(shader.get_uniform("this_t"), actual_draw_fraction);
    glBindVertexArray(M_vertex_array);
    glDrawElements(GL_TRIANGLES, M_index_size, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
