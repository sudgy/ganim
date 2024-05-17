#include "drawable_object.hpp"

#include "ganim/gl/gl.hpp"
#include "ganim/gl/framebuffer.hpp"

#include "ganim/util/alpha_threshold.hpp"
#include "ganim/util/distance_transform.hpp"

using namespace ganim;

namespace {
    gl::Shader make_shader()
    {
        auto vertex = gl::Shader::Source();
        vertex.add_source(
            "#version 330 core\n"
            "layout (location = 0) in vec2 in_pos;\n"
            "layout (location = 1) in vec2 in_tex_coord;\n"
            "out vec2 out_tex_coord;\n"
#include "ganim/shaders/basic_shader_vertex_uniforms.glsl"
#include "ganim/shaders/basic_shader_vertex_functions.glsl"
R"(
void main()
{
    vec4[2] r = rotor_mult(model, view);
    vec4 pos = vec4(rotor_trivector_sandwich(r, vec3(in_pos, 0) *scale), 1.0);
    pos.w = -pos.z;
    pos.x *= camera_scale.x;
    pos.y *= -camera_scale.y;
    pos.z *= pos.z / 1024;
    gl_Position = pos;
    out_tex_coord = in_tex_coord;
}
)"      );
        auto fragment = gl::Shader::Source();
        fragment.add_source(
R"(
#version 330 core

in vec2 out_tex_coord;

uniform float thickness;
uniform vec4 color;
uniform sampler2D distance_transform;

out vec4 out_color;

void main()
{
    float distance = texture(distance_transform, out_tex_coord).r;
    out_color = vec4(color.xyz, clamp(thickness + 0.5 - distance, 0, 1));
}
)"
        );
        return gl::Shader(vertex, fragment);
    }
    gl::Shader& get_shader()
    {
        static auto result = make_shader();
        return result;
    }
}

void DrawableObject::draw_outline(const Camera& camera)
{
    if (M_outline_thickness == 0.0) return;
    if (!M_outline_texture or M_always_invalidate_outline) {
        create_outline(camera);
    }

    // Draw the outline
    auto current_viewport = std::array<int, 4>{0};
    glGetIntegerv(GL_VIEWPORT, current_viewport.data());
    const auto camera_width = camera.get_starting_width();
    const auto gtp = current_viewport[2] / camera_width;
    const auto thickness = gtp * M_outline_thickness;

    auto& shader = get_shader();
    glUseProgram(shader);
    glUniform2f(shader.get_uniform("camera_scale"),
                camera.get_x_scale(), camera.get_y_scale());
    shader.set_rotor_uniform("view", ~camera.get_rotor());
    shader.set_rotor_uniform("model", get_rotor());
    glUniform1f(shader.get_uniform("scale"), get_scale());
    glUniform4f(shader.get_uniform("color"),
            M_outline_color.r / 255.0, M_outline_color.g / 255.0,
            M_outline_color.b / 255.0, M_outline_color.a / 255.0);
    glUniform1f(shader.get_uniform("thickness"), thickness);
    glBindVertexArray(M_outline_vertex_array);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, M_outline_texture);
    glUniform1i(shader.get_uniform("distance_transform"), 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void DrawableObject::set_outline(const Color& color, double thickness)
{
    M_outline_color = color;
    if (thickness != M_outline_thickness) {
        M_outline_thickness = thickness;
        M_outline_texture = 0;
    }
}

void DrawableObject::invalidate_outline()
{
    M_outline_texture = 0;
}

void DrawableObject::set_draw_fraction(double value)
{
    Object::set_draw_fraction(value);
    invalidate_outline();
}

void DrawableObject::create_outline(const Camera& camera)
{
    auto rotor = get_rotor();
    auto scale = get_scale();
    apply_rotor(~rotor);
    this->scale(1/scale);
    auto bounding_box = get_true_bounding_box();
    using namespace vga3;
    const auto x1 = bounding_box.p1.blade_project<e1>();
    const auto x2 = bounding_box.p2.blade_project<e1>();
    const auto y1 = bounding_box.p1.blade_project<e2>();
    const auto y2 = bounding_box.p2.blade_project<e2>();
    const auto z1 = bounding_box.p1.blade_project<e3>();
    const auto z2 = bounding_box.p2.blade_project<e3>();
    {
        if (z2 - z1 > std::max(x2 - x1, y2 - y1) * 1e-10) {
            apply_rotor(rotor);
            throw std::runtime_error("An outline was attempted to be drawn on "
                    "an object that seems to have 3D extent.");
        }
    }
    const auto bigger_width = x2 - x1 + M_outline_thickness * 3;
    const auto bigger_height = y2 - y1 + M_outline_thickness * 3;
    const auto size_base = std::max(bigger_width, bigger_height);

    auto current_draw_framebuffer = 0;
    auto current_read_framebuffer = 0;
    auto current_viewport = std::array<int, 4>{0};
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &current_draw_framebuffer);
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &current_read_framebuffer);
    glGetIntegerv(GL_VIEWPORT, current_viewport.data());
    const auto camera_width = camera.get_starting_width();
    const auto gtp = current_viewport[2] / camera_width;
    const auto texture_size
        = std::bit_ceil(static_cast<unsigned>(size_base * gtp));
    const auto size = texture_size / gtp;

    auto framebuffer = gl::Framebuffer();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    auto framebuffer_texture = gl::Texture();
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, texture_size, texture_size, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           framebuffer_texture, 0);
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Error: Framebuffer is not complete when "
                "drawing an outline.");
    }
    glViewport(0, 0, texture_size, texture_size);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    auto fake_camera = Camera(20, size, -size);
    fake_camera.shift((x1 + x2)/2*e1 + (y1 + y2)/2*e2);

    draw(fake_camera);

    M_outline_texture = distance_transform(
        alpha_threshold(
            framebuffer_texture,
            0.1,
            texture_size,
            texture_size
        ),
        texture_size
    );

    glBindTexture(GL_TEXTURE_2D, M_outline_texture);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR
    );
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    M_outline_vertex_array = gl::VertexArray();
    M_outline_vertex_buffer = gl::Buffer();
    glBindVertexArray(M_outline_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, M_outline_vertex_buffer);
    struct Vertex {
        float x;
        float y;
        float tx;
        float ty;
    };
    auto new_x1 = (x1 + x2) / 2 - size / 2;
    auto new_x2 = (x1 + x2) / 2 + size / 2;
    auto new_y1 = (y1 + y2) / 2 - size / 2;
    auto new_y2 = (y1 + y2) / 2 + size / 2;
    auto vertices = std::array<Vertex, 4>{{
        {static_cast<float>(new_x1), static_cast<float>(new_y2), 0, 1},
        {static_cast<float>(new_x2), static_cast<float>(new_y2), 1, 1},
        {static_cast<float>(new_x1), static_cast<float>(new_y1), 0, 0},
        {static_cast<float>(new_x2), static_cast<float>(new_y1), 1, 0}
    }};
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(Vertex)*4,
        vertices.data(),
        GL_DYNAMIC_DRAW
    );
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    if (!M_outline_element_buffer) {
        M_outline_element_buffer = gl::Buffer();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_outline_element_buffer);
        auto indices = std::array<unsigned, 6>{{
            0, 1, 2, 2, 1, 3
        }};
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(unsigned)*6,
            indices.data(),
            GL_STATIC_DRAW
        );
    }
    else {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_outline_element_buffer);
    }
    glBindVertexArray(0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, current_draw_framebuffer);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, current_read_framebuffer);
    glViewport(
        current_viewport[0], current_viewport[1],
        current_viewport[2], current_viewport[3]
    );
    this->scale(scale);
    apply_rotor(rotor);
}
