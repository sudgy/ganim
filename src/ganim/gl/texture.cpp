#include "texture.hpp"

#include "ganim/gl/gl.hpp"

using namespace ganim::gl;

Texture::Texture(std::uint8_t* data, int width, int height)
{
    glGenTextures(1, &M_id);
    glBindTexture(GL_TEXTURE_2D, M_id);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR
    );
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR
    );
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, data
    );
    glGenerateMipmap(GL_TEXTURE_2D);
}

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
