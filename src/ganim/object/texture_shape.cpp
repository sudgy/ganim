#include "texture_shape.hpp"

#include "ganim/gl/gl.hpp"

#include "shaders.hpp"

using namespace ganim;

gl::Shader& texture_shape_helper::get_shader(
    bool is_creating,
    double noise_creating
)
{
    using enum ShaderFeature;
    auto flags = Time | VertexColors | Texture;
    if (is_creating) flags |= Create;
    if (noise_creating) flags |= NoiseCreate;
    auto& result = ganim::get_shader(flags);
    glUseProgram(result);
    glUniform1i(result.get_uniform("in_texture"), 0);
    return result;
}

void texture_shape_helper::set_texture(unsigned texture)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void texture_shape_helper::buffer_vertices(
    const Shape& self,
    const std::vector<TextureVertex>& texture_vertices
)
{
    const auto vsize = sizeof(Shape::Vertex) * self.get_vertices().size();
    const auto tsize = sizeof(TextureVertex) * texture_vertices.size();
    glBufferData(GL_ARRAY_BUFFER, vsize + tsize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vsize, &self.get_vertices()[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vsize, tsize, &texture_vertices[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Shape::Vertex),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Shape::Vertex),
                          reinterpret_cast<void*>(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Shape::Vertex),
                          reinterpret_cast<void*>(4*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(TextureVertex),
                          reinterpret_cast<void*>(vsize));
    glEnableVertexAttribArray(3);
}
