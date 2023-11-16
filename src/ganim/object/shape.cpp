#include "shape.hpp"

#include "ganim/gl/gl.hpp"

using namespace ganim;

// I promise I'll make this better in the future
namespace {
    gl::Shader::Source vertex_shader()
    {
        auto source = gl::Shader::Source();
        source.add_source(R"(
#version 330 core
layout (location = 0) in vec3 in_pos;
uniform vec2 camera_scale;
void main()
{
    vec4 pos = vec4(in_pos, 1.0);
    pos.z -= 20; // This is the camera rotor but I haven't implemented it yet
    pos.w = -pos.z;
    pos.x *= camera_scale.x;
    pos.y *= camera_scale.y;
    pos.z /= 1024; // I'll make this better later
    gl_Position = pos;
})"
);
        return source;
    }
    gl::Shader::Source fragment_shader()
    {
        auto source = gl::Shader::Source();
        source.add_source(R"(
#version 330 core
out vec4 color;
void main()
{
    color = vec4(1.0, 1.0, 1.0, 1.0);
})"
);
        return source;
    }
}

Shape::Shape(
    const std::vector<float>& vertices,
    const std::vector<unsigned> indices
) : M_shader(vertex_shader(), fragment_shader()),
    M_index_size(indices.size())
{
    glBindVertexArray(M_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, M_vertex_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_element_buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(),
                 vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*indices.size(),
                 indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Shape::draw()
{
    glUseProgram(M_shader);
    glUniform2f(M_shader.get_uniform("camera_scale"), 40/16.0f, 40/9.0f);
    glBindVertexArray(M_vertex_array);
    glDrawElements(GL_TRIANGLES, M_index_size, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
