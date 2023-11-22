#include "base.hpp"

#include <stdexcept>
#include <cmath>
#include <SFML/Window.hpp>

#include "ganim/gl/gl.hpp"
#include "ganim/object/shaders.hpp"

// I may need to do something else later, but for now this should work to get a
// valid context
namespace {
    auto settings = sf::ContextSettings(24, 0, 0, 3, 3);
    auto context = sf::Context(settings, 1, 1);
}

using namespace ganim;

SceneBase::SceneBase(
    int pixel_width,
    int pixel_height,
    double coord_width,
    double coord_height,
    int fps
)
:   M_pixel_width(pixel_width),
    M_pixel_height(pixel_height),
    M_fps(fps),
    M_camera(20, coord_width, coord_height)
{
    add(M_camera);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
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
    glBindTexture(GL_TEXTURE_2D, M_depth_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 pixel_width, pixel_height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           M_depth_buffer, 0);
}

void SceneBase::frame_advance()
{
    for (auto object : M_objects) {
        object->update();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, M_framebuffer);
    glViewport(0, 0, M_pixel_width, M_pixel_height);
    glClearColor(
        M_background_color.r / 255.0,
        M_background_color.g / 255.0,
        M_background_color.b / 255.0,
        1
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto& shader = shape_shader();
    glUseProgram(shader);
    glUniform2f(shader.get_uniform("camera_scale"),
                M_camera.get_x_scale(), M_camera.get_y_scale());
    shader.set_rotor_uniform("view", ~M_camera.get_rotor());
    for (auto object : M_drawables) {
        if (object->is_visible()) {
            object->draw();
        }
    }
    process_frame();
}

void SceneBase::frame_advance(int amount)
{
    if (amount < 0) {
        throw std::invalid_argument(
            "Negative amount passed to SceneBase::frame_advance");
    }
    for (int i = 0; i < amount; ++i) {
        frame_advance();
    }
}

void SceneBase::wait(double time)
{
    if (time < 0) {
        throw std::invalid_argument("Negative time passed to SceneBase::wait");
    }
    auto amount = static_cast<int>(std::round(time * M_fps));
    frame_advance(amount);
}

void SceneBase::add(Animatable& object)
{
    if (auto p = dynamic_cast<Drawable*>(&object)) add(*p);
    else add_animatable(object);
}

void SceneBase::add(Drawable& object)
{
    add_animatable(object);
    M_drawables.emplace_back(&object);
}

void SceneBase::add_animatable(Animatable& object)
{
    object.set_fps(M_fps);
    M_objects.emplace_back(&object);
}
