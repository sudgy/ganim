#include "base.hpp"

#include <stdexcept>
#include <cmath>
#include <format>
#include <ranges>
#include <SFML/Window.hpp>

#include "ganim/gl/gl.hpp"
#include "ganim/util/stb_image.h"

#include "ganim/object/texture_shape.hpp"

using namespace ganim;

// I may need to do something else later, but for now this should work to get a
// valid context
namespace {
    auto settings = sf::ContextSettings(24, 0, 4, 4, 3, sf::ContextSettings::Debug);
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

    auto layer_vertices = std::array{
         1.0f,  1.0f,
        -1.0f,  1.0f,
         1.0f, -1.0f,
        -1.0f, -1.0f,
    };
    auto layer_indices = std::array{
        0U, 1U, 3U,
        0U, 3U, 2U
    };
    auto layer_shader = []{
        auto vertex = gl::Shader::Source();
        auto fragment = gl::Shader::Source();
        vertex.add_source(R"(
#version 330 core

layout (location = 0) in vec2 in_pos;

void main()
{
    gl_Position = vec4(in_pos.xy, 1, 1);
}
        )");
        fragment.add_source(R"(
#version 330 core

out vec4 color;

uniform sampler2DMS texture;

void main()
{
    ivec2 pos = ivec2(round(gl_FragCoord.x - 0.5), round(gl_FragCoord.y - 0.5));
    color = vec4(0);
    for (int i = 0; i < 4; ++i) {
        color += texelFetch(texture, pos, i);
    }
    color /= 4;
}
        )");
        return gl::Shader(vertex, fragment);
    }();
}

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
    M_camera(20, coord_width, coord_height),
    M_static_camera(20, coord_width, coord_height)
{
    add(M_camera);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, M_framebuffer_texture);
    glTexImage2DMultisample(
        GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA32F, pixel_width, pixel_height,
        GL_TRUE
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, M_framebuffer);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE,
        M_framebuffer_texture, 0
    );
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Error: Framebuffer is not complete.");
    }

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, M_depth_buffer);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH_COMPONENT,
                 pixel_width, pixel_height, GL_TRUE);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE,
        M_depth_buffer, 0
    );

    glBindTexture(GL_TEXTURE_2D, M_downsampled_framebuffer_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pixel_width, pixel_height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, M_downsampled_framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           M_downsampled_framebuffer_texture, 0);
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error(
                "Error: Downsampled framebuffer is not complete.");
    }
}

void SceneBase::frame_advance()
{
    update();
    auto objects_copy = M_objects;
    for (auto object : objects_copy) {
        object->update();
    }
    auto ghosts_copy = std::vector<Animatable*>();
    ghosts_copy.reserve(M_ghost_animating_objects.size());
    for (auto& object : M_ghost_animating_objects) {
        ghosts_copy.push_back(&*object);
    }
    for (auto object : ghosts_copy) {
        object->update();
    }
    if (M_animating) {
        glViewport(0, 0, M_pixel_width, M_pixel_height);
        if (!M_depth_layers.empty()) {
            glDisable(GL_BLEND);
            for (int i = 0; i < ssize(M_depth_layers); ++i) {
                auto& l = M_depth_layers[i];
                glBindFramebuffer(GL_FRAMEBUFFER, l.framebuffer);
                glClearColor(0, 0, 0, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                for (auto object : M_drawables) {
                    if (object->is_visible()) {
                        if (!object->is_fixed_in_frame()) {
                            if (i == 0) {
                                object->set_peeling_depth_buffer(nullptr);
                            }
                            else {
                                object->set_peeling_depth_buffer(
                                    &M_depth_layers[i-1].depth_buffer
                                );
                            }
                            object->draw_outline(M_camera);
                            object->draw(M_camera);
                        }
                    }
                }
            }
            glEnable(GL_BLEND);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, M_framebuffer);
        glClearColor(
            M_background_color.r / 255.0,
            M_background_color.g / 255.0,
            M_background_color.b / 255.0,
            1
        );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (M_background_object) {
            M_background_object->draw(M_static_camera);
            glClear(GL_DEPTH_BUFFER_BIT);
        }
        if (!M_depth_layers.empty()) {
            for (auto& layer : std::views::reverse(M_depth_layers)) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, layer.texture);
                glUseProgram(layer_shader);
                glBindVertexArray(layer.vertex_array);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
                glBindVertexArray(0);
                glClear(GL_DEPTH_BUFFER_BIT);
            }
        }
        else {
            for (auto object : M_drawables) {
                object->set_peeling_depth_buffer(nullptr);
                if (object->is_visible()) {
                    if (!object->is_fixed_in_frame()) {
                        object->draw_outline(M_camera);
                        object->draw(M_camera);
                    }
                }
            }
        }
        glClear(GL_DEPTH_BUFFER_BIT);
        for (auto object : M_drawables) {
            if (object->is_visible()) {
                if (object->is_fixed_in_frame()) {
                    object->draw_outline(M_static_camera);
                    object->draw(M_static_camera);
                }
            }
        }
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, M_downsampled_framebuffer);
        glBlitFramebuffer(
            0, 0, M_pixel_width, M_pixel_height,
            0, 0, M_pixel_width, M_pixel_height,
            GL_COLOR_BUFFER_BIT, GL_NEAREST
        );
        glBindFramebuffer(GL_FRAMEBUFFER, M_downsampled_framebuffer);
        process_frame();
    }
    ++M_frame_count;
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

int SceneBase::get_frame_count() const
{
    return M_frame_count;
}

double SceneBase::get_time() const
{
    return double(M_frame_count) / M_fps;
}

void SceneBase::stop_animating()
{
    M_animating = false;
}

void SceneBase::start_animating()
{
    M_animating = true;
}

void SceneBase::add_animatable(Animatable& object)
{
    if (std::ranges::find(M_objects, &object) == M_objects.end()) {
        object.set_fps(M_fps);
        M_objects.emplace_back(&object);
    }
}

void SceneBase::add_drawable(Object& object)
{
    if (std::ranges::find(M_drawables, &object) == M_drawables.end()) {
        M_drawables.emplace_back(&object);
    }
}

void SceneBase::add_group(Group& object)
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

void SceneBase::remove_drawable(Object& object)
{
    auto it = std::ranges::find(M_drawables, &object);
    if (it != M_drawables.end()) {
        M_drawables.erase(it);
    }
}

void SceneBase::remove_group(Group& object)
{
    for (auto* obj : object) {
        remove(*obj);
    }
}

void SceneBase::add_for_animation(MaybeOwningRef<Animatable> object)
{
    if (&*object == &M_camera) return;
    for (auto old_object : M_objects) {
        if (old_object == &*object) return;
    }
    object->set_fps(M_fps);
    M_ghost_animating_objects.push_back(std::move(object));
}

void SceneBase::remove_for_animation(Animatable& object)
{
    auto it = std::ranges::find_if(M_ghost_animating_objects, [&](auto& o){
        return &*o == &object;
    });
    if (it != M_ghost_animating_objects.end()) {
        M_ghost_animating_objects.erase(it);
    }
}

void SceneBase::set_transparency_layers(int layers)
{
    auto old_size = M_depth_layers.size();
    M_depth_layers.resize(layers);
    for (int i = old_size; i < layers; ++i) {
        auto& l = M_depth_layers[i];
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, l.texture);
        glTexImage2DMultisample(
            GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA32F,
            M_pixel_width, M_pixel_height, GL_TRUE
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindFramebuffer(GL_FRAMEBUFFER, l.framebuffer);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE,
            l.texture, 0
        );

        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, l.depth_buffer);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4,
            GL_DEPTH_COMPONENT, M_pixel_width, M_pixel_height, GL_TRUE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE,
            l.depth_buffer, 0
        );

        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Error: Framebuffer is not complete.");
        }

        glBindVertexArray(l.vertex_array);
        glBindBuffer(GL_ARRAY_BUFFER, l.vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(layer_vertices),
                     layer_vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2,
                              reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, l.element_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(layer_indices),
                     layer_indices.data(), GL_STATIC_DRAW);
        glBindVertexArray(0);
    }
}

void SceneBase::set_background_image(const std::string& filename)
{
    // TODO: When you make image objects use those instead
    int width, height, channels;
    auto image = stbi_load(filename.c_str(), &width, &height, &channels, 4);
    if (image == nullptr) {
        throw std::runtime_error("Error loading background image " + filename);
    }
    M_background_texture = gl::Texture(image, width, height);
    stbi_image_free(image);
    auto w = static_cast<float>(M_camera.get_starting_width()/2);
    auto h = static_cast<float>(M_camera.get_starting_height()/2);
    auto background_object = std::make_unique<TextureShape<Shape>>(
        std::vector<Shape::Vertex>{{ w,  h, 0},
         { w, -h, 0},
         {-w, -h, 0},
         {-w,  h, 0}},
         std::vector<unsigned>{0, 1, 2, 0, 2, 3}
    );
    background_object->set_texture_vertices(
        {{ 0,  0},
         { 0, -1},
         {-1, -1},
         {-1,  0}}
    );
    background_object->set_texture(M_background_texture);
    background_object->set_visible(true);
    M_background_object = std::move(background_object);
}
