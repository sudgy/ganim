#include <iostream>
#include <array>

#include <SFML/Window.hpp>
#include "ganim/gl/gl.hpp"

#include "ganim/gl/texture.hpp"
#include "ganim/gl/framebuffer.hpp"

#include "ganim/video_writer/video_writer.hpp"

using namespace ganim;

int main()
{
    constexpr int width = 256;
    constexpr int height = 144;
    auto settings = sf::ContextSettings(24, 0, 0, 3, 3);
    auto context = sf::Context(settings, width, height);
    auto framebuffer = gl::Framebuffer();
    auto texture = gl::Texture();
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture, 0);
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete.  Its actual "
                  << "status is " << status << ".\n";
    }
    glViewport(0, 0, width, height);
    auto data_heap = std::make_unique<std::array<unsigned char, width*height*3>>();
    auto& data = *data_heap;
    auto writer = VideoWriter("Test.mp4", width, height, 60);

    for (int i = 0; i < 120; ++i) {
        glClearColor(0, i / 120.0, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data.data());
        writer.write_frame(data);
    }
}
