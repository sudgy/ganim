#include "base.hpp"

#include <stdexcept>
#include <cmath>
#include <format>
#include <SFML/Window.hpp>

#include "ganim/gl/gl.hpp"

// I may need to do something else later, but for now this should work to get a
// valid context
namespace {
    auto settings = sf::ContextSettings(24, 0, 0, 4, 3, sf::ContextSettings::Debug);
    auto context = sf::Context(settings, 1, 1);
    void debug_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void*
    )
    {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
        throw std::runtime_error(std::format(
            "OpenGL Error:\n"
            "Source: {}\n"
            "Type: {}\n"
            "ID: {}\n"
            "Severity: {}\n"
            "Message: {}\n",
            source, type, id, severity, std::string_view(message, length)));
    }
    auto stupid = []{
        glDebugMessageCallback(debug_callback, nullptr);
        return 1;
    }();
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pixel_width, pixel_height, 0,
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
    update();
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
    for (auto object : M_drawables) {
        if (object->is_visible()) {
            object->draw_outline(M_camera);
            object->draw(M_camera);
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

void SceneBase::add_animatable(Animatable& object)
{
    if (std::ranges::find(M_objects, &object) == M_objects.end()) {
        object.set_fps(M_fps);
        M_objects.emplace_back(&object);
    }
}

void SceneBase::add_drawable(Drawable& object)
{
    if (std::ranges::find(M_drawables, &object) == M_drawables.end()) {
        M_drawables.emplace_back(&object);
    }
}

void SceneBase::add_group(GroupBase& object)
{
    for (auto* obj : object) {
        add(*obj);
    }
}

void SceneBase::remove_animatable(Animatable& object)
{
    auto it = std::ranges::find(M_objects, &object);
    if (it != M_objects.end()) {
        M_objects.erase(it);
    }
}

void SceneBase::remove_drawable(Drawable& object)
{
    auto it = std::ranges::find(M_drawables, &object);
    if (it != M_drawables.end()) {
        M_drawables.erase(it);
    }
}

void SceneBase::remove_group(GroupBase& object)
{
    for (auto* obj : object) {
        remove(*obj);
    }
}
