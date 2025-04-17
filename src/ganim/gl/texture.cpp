#include "texture.hpp"

#include <vector>

#include "ganim/gl/gl.hpp"
#include "ganim/util/stb_image_write.h"

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
        GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0,
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
        if (M_id) glDeleteTextures(1, &M_id);
        M_id = other.M_id;
        other.M_id = 0;
    }
    return *this;
}

void Texture::write_to_file(std::string filename)
{
    int w = -1;
    int h = -1;
    glGetTextureLevelParameteriv(M_id, 0, GL_TEXTURE_WIDTH, &w);
    glGetTextureLevelParameteriv(M_id, 0, GL_TEXTURE_HEIGHT, &h);
    auto buffer = std::vector<std::uint8_t>();
    buffer.resize(w*h*4);
    glGetTextureImage(M_id, 0, GL_RGBA, GL_UNSIGNED_BYTE, w*h*4, buffer.data());
    stbi_write_png(filename.c_str(), w, h, 4, buffer.data(), w * 4);
}
