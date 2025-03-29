#include "single_object.hpp"

#include "ganim/gl/gl.hpp"
#include "ganim/gl/framebuffer.hpp"
#include "ganim/gl/shader.hpp"

#include "ganim/object/shaders.hpp"

#include "ganim/util/alpha_threshold.hpp"
#include "ganim/util/distance_transform.hpp"

using namespace ganim;

SingleObject::SingleObject(const SingleObject& other)
    : Object(other),
    M_outline_color(other.M_outline_color),
    M_always_invalidate_outline(other.M_always_invalidate_outline)
    // The OpenGL stuff gets made whenever it's not there and is needed so we
    // don't have to copy it here
{}

void SingleObject::draw_outline(const Camera& camera)
{
    if (get_scale() == 0) return;
    if (M_outline_thickness == 0.0) return;
    if (!M_outline_texture or M_always_invalidate_outline) {
        create_outline(camera);
    }

    auto current_viewport = std::array<int, 4>{0};
    glGetIntegerv(GL_VIEWPORT, current_viewport.data());
    const auto camera_width = camera.get_starting_width();
    const auto gtp = current_viewport[2] / camera_width;
    const auto thickness = gtp * M_outline_thickness / get_scale();

    auto features = ShaderFeature::Outline;
    if (peeling_depth_buffer()) features |= ShaderFeature::DepthPeeling;
    auto& shader = get_shader(features);
    glUseProgram(shader);
    if (auto buffer = peeling_depth_buffer()) {
        glUniform1i(shader.get_uniform("layer_depth_buffer"), 15);
        glActiveTexture(GL_TEXTURE15);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, *buffer);
    }
    glUniform2f(shader.get_uniform("camera_scale"),
                camera.get_x_scale(), camera.get_y_scale());
    auto view = ~camera.get_rotor();
    shader.set_rotor_uniform("view", view);
    auto model = get_rotor();
    if (is_fixed_orientation()) {
        using namespace pga3;
        auto view_euclidean =
            view.blade_project<e>() +
            view.blade_project<e12>() * e12 +
            view.blade_project<e13>() * e13 +
            view.blade_project<e23>() * e23;
        model = ~view_euclidean * model;
    }
    shader.set_rotor_uniform("model", model);
    glUniform1f(shader.get_uniform("scale"), get_scale());
    glUniform1f(shader.get_uniform("depth_z"), get_depth_z());
    auto color = M_outline_color;
    color.a = get_color().a * get_opacity();
    glUniform4f(shader.get_uniform("object_color"),
            color.r / 255.0, color.g / 255.0, color.b / 255.0, color.a / 255.0);
    glUniform1f(shader.get_uniform("thickness"), thickness);
    glBindVertexArray(M_outline_vertex_array);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, M_outline_texture);
    glUniform1i(shader.get_uniform("distance_transform"), 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void SingleObject::set_outline(const Color& color, double thickness)
{
    M_outline_color = color;
    if (thickness != M_outline_thickness) {
        M_outline_thickness = thickness;
        M_outline_texture = 0;
    }
}

void SingleObject::invalidate_outline()
{
    M_outline_texture = 0;
}

Color SingleObject::get_outline_color() const
{
    return M_outline_color;
}

double SingleObject::get_outline_thickness() const
{
    return M_outline_thickness;
}

void SingleObject::set_draw_fraction(double value)
{
    auto different = value != get_draw_fraction();
    Object::set_draw_fraction(value);
    if (different) invalidate_outline();
}

void SingleObject::create_outline(const Camera& camera)
{
    auto rotor = get_rotor();
    auto scale = get_scale();
    auto opacity = get_opacity();
    apply_rotor(~rotor);
    this->scale(1/scale);
    set_opacity(1);
    auto bounding_box = get_true_bounding_box();
    using namespace pga3;
    auto p1 = bounding_box.get_inner_lower_left_vertex().undual();
    auto p2 = bounding_box.get_outer_upper_right_vertex().undual();
    const auto x1 = p1.blade_project<e1>();
    const auto x2 = p2.blade_project<e1>();
    const auto y1 = p1.blade_project<e2>();
    const auto y2 = p2.blade_project<e2>();
    const auto z1 = p1.blade_project<e3>();
    const auto z2 = p2.blade_project<e3>();
    {
        if (z2 - z1 > std::max(x2 - x1, y2 - y1) * 1e-10) {
            apply_rotor(rotor);
            this->scale(scale);
            throw std::runtime_error("An outline was attempted to be drawn on "
                    "an object that seems to have 3D extent.");
        }
    }
    const auto bigger_width = x2 - x1 + M_outline_thickness * 3 / scale;
    const auto bigger_height = y2 - y1 + M_outline_thickness * 3 / scale;
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
        = std::max(std::bit_ceil(static_cast<unsigned>(size_base * gtp)), 8U);
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
            0.03,
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
    set_opacity(opacity);
}
