#include "framebuffer.hpp"

#include "ganim/gl/gl.hpp"

using namespace ganim::gl;

Framebuffer::Framebuffer()
{
    glGenFramebuffers(1, &M_id);
}

Framebuffer::~Framebuffer()
{
    if (M_id) glDeleteFramebuffers(1, &M_id);
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept :
    M_id(other.M_id)
{
    other.M_id = 0;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
{
    if (this != &other) {
        if (M_id) glDeleteFramebuffers(1, &M_id);
        M_id = other.M_id;
        other.M_id = 0;
    }
    return *this;
}
