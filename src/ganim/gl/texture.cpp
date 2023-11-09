#include "texture.hpp"

#include "ganim/gl/gl.hpp"

using namespace ganim::gl;

Texture::Texture()
{
    glGenTextures(1, &M_id);
}

Texture::~Texture()
{
    if (M_id) glDeleteTextures(1, &M_id);
}

Texture::Texture(Texture&& other) noexcept : M_id(other.M_id)
{
    other.M_id = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other) {
        M_id = other.M_id;
        other.M_id = 0;
    }
    return *this;
}
