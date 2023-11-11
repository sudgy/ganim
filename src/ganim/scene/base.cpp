#include "base.hpp"

#include <stdexcept>

#include "ganim/gl/gl.hpp"

using namespace ganim;

SceneBase::SceneBase(
    int pixel_width,
    int pixel_height
)
:   M_pixel_width(pixel_width),
    M_pixel_height(pixel_height)
{
    glBindTexture(GL_TEXTURE_2D, M_framebuffer_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pixel_width, pixel_height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, M_framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           M_framebuffer_texture, 0);
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Error: Framebuffer is not complete.");
    }
}

void SceneBase::frame_advance()
{
    glBindFramebuffer(GL_FRAMEBUFFER, M_framebuffer);
    glViewport(0, 0, M_pixel_width, M_pixel_height);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    process_frame();
}
