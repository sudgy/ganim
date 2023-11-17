#include "shape.hpp"

#include "ganim/gl/gl.hpp"

#include "shaders.hpp"

using namespace ganim;

Shape::Shape(
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned> indices
) : M_index_size(indices.size())
{
    glBindVertexArray(M_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, M_vertex_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_element_buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(),
                 &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*indices.size(),
                 indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Shape::draw()
{
    auto& shader = shape_shader();
    glUseProgram(shader);
    shader.set_rotor_uniform("model", get_rotor());
    glBindVertexArray(M_vertex_array);
    glDrawElements(GL_TRIANGLES, M_index_size, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
