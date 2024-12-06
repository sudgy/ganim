#include "transform.hpp"

#include <ranges>

#include "ganim/scene/base.hpp"
#include "ganim/animation/animation.hpp"
#include "ganim/object/bases/single_object.hpp"
#include "ganim/gl/gl.hpp"
#include "ganim/object/shaders.hpp"
#include "ganim/math.hpp"

#include "ganim/util/alpha_threshold.hpp"
#include "ganim/util/distance_transform.hpp"
#include "ganim/util/discrete_interpolate.hpp"

using namespace ganim;

namespace {
    gl::Shader make_shader()
    {
        auto vertex = gl::Shader::Source();
        vertex.add_source(
R"(
#version 330 core
layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec2 in_tex_coord1;
layout (location = 2) in vec2 in_tex_coord2;
out vec2 out_tex_coord1;
out vec2 out_tex_coord2;

uniform vec2 camera_scale;
uniform vec4 view[2];
uniform vec4 model[2];
uniform float scale;

// This calculates the sandwich product of a 3D PGA rotor r and a trivector p
// (~r * p * r).  p corresponds to the trivector (p + e0).dual(), and r
// represents a rotor under the correspondence {{a, b, c, d}, {x, y, z, w}} <->
// a + b e23 + c e31 + d e12 + x e01 + y e02 + z e03 + w e0123.
//
// This way of calculating it is due to Steven De Keninck in his upcoming paper
// "FPGA".
vec3 rotor_trivector_sandwich(vec4 r[2], vec3 p)
{
    vec3 t = cross(r[0].yzw, p) + r[1].xyz;
    return 2 * (r[0].x * t + cross(r[0].yzw, t) + r[0].yzw * r[1].w) + p;
}

// Calculates a rotor product.  The format of the rotors is the same as above.
vec4[2] rotor_mult(vec4 m[2], vec4 n[2])
{
    float a = m[0].x;
    vec3 b = m[0].yzw;
    vec3 c = m[1].xyz;
    float d = m[1].w;
    float x = n[0].x;
    vec3 y = n[0].yzw;
    vec3 z = n[1].xyz;
    float w = n[1].w;
    return vec4[2](
        vec4(a*x - dot(b, y), a*y + x*b + cross(y, b)),
        vec4(a*z - w*b + x*c - d*y + cross(z, b) + cross(y, c),
             a*w + d*x + dot(b, z) + dot(c, y))
    );
}

void main()
{
    vec4[2] r = rotor_mult(model, view);
    vec4 pos = vec4(rotor_trivector_sandwich(r, vec3(in_pos, 0)), 1.0);
    pos.w = -pos.z;
    pos.x *= camera_scale.x;
    pos.y *= -camera_scale.y;
    pos.z *= pos.z / 1024;
    gl_Position = pos;
    out_tex_coord1 = in_tex_coord1;
    out_tex_coord2 = in_tex_coord2;
}
)"      );
        auto fragment = gl::Shader::Source();
        fragment.add_source(
R"(
#version 330 core

in vec2 out_tex_coord1;
in vec2 out_tex_coord2;

uniform sampler2D object1;
uniform sampler2D distance_transform1;
uniform sampler2D object2;
uniform sampler2D distance_transform2;
uniform float t;
uniform float scale1;
uniform float scale2;

out vec4 out_color;

void main()
{
    vec4 color1 = texture(object1, out_tex_coord1);
    vec4 color2 = texture(object2, out_tex_coord2);
    float distance1 = texture(distance_transform1, out_tex_coord1).r;
    float distance2 = texture(distance_transform2, out_tex_coord2).r;
    if (distance2 > scale1 * (1 - t)) color1.a = 0;
    if (distance1 > scale2 * t) color2.a = 0;
    if (color1.a > 0 && color2.a > 0) {
        color1 /= sqrt(color1.a);
        color2 /= sqrt(color2.a);
        out_color = vec4(
            mix(color1.rgb, color2.rgb, t),
            max(color1.a, color2.a)
        );
    }
    else if (color1.a > 0) {
        color1 /= sqrt(color1.a);
        out_color = color1;
    }
    else if (color2.a > 0) {
        color2 /= sqrt(color2.a);
        out_color = color2;
    }
    else {
        out_color = vec4(0, 0, 0, 0);
    }
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
    struct StaticPart : public Animatable {
        std::optional<MaybeOwningRef<Object>> M_tracked_object;
        Object& tracked_object() {return **M_tracked_object;}
        gl::Texture M_object_texture = 0;
        gl::Texture M_distance_transform = 0;
        Box M_bounding_box;
        double M_x1 = 0;
        double M_x2 = 0;
        double M_y1 = 0;
        double M_y2 = 0;
        int M_texture_size = 0;
        int M_texture_width = 0;
        int M_texture_height = 0;
        std::pair<unsigned, unsigned> get_textures(const Camera& camera)
        {
            if (M_object_texture == 0) generate_textures(camera);
            return {M_object_texture, M_distance_transform};
        }
        void generate_textures(const Camera& camera)
        {
            M_bounding_box = tracked_object().get_true_bounding_box();
            using namespace pga3;
            auto p1 = M_bounding_box.get_inner_lower_left_vertex().undual();
            auto p2 = M_bounding_box.get_outer_upper_right_vertex().undual();
            const auto x1 = p1.blade_project<e1>();
            const auto x2 = p2.blade_project<e1>();
            const auto y1 = p1.blade_project<e2>();
            const auto y2 = p2.blade_project<e2>();
            const auto z1 = p1.blade_project<e3>();
            const auto z2 = p2.blade_project<e3>();
            {
                if (z2 - z1 > std::max(x2 - x1, y2 - y1) * 1e-10) {
                    throw std::runtime_error("A texture transform was attempted"
                            " on an object that seems to have 3D extent.");
                }
            }
            const auto size_base = std::max(x2 - x1, y2 - y1);

            auto current_draw_framebuffer = 0;
            auto current_read_framebuffer = 0;
            auto current_viewport = std::array<int, 4>{0};
            glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &current_draw_framebuffer);
            glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &current_read_framebuffer);
            glGetIntegerv(GL_VIEWPORT, current_viewport.data());
            const auto camera_width = camera.get_starting_width();
            const auto gtp = current_viewport[2] / camera_width;
            M_texture_size = std::max(
                int(std::bit_ceil(static_cast<unsigned>(size_base * gtp))), 8);
            M_texture_width = (x2 - x1) * gtp;
            M_texture_height = (y2 - y1) * gtp;
            const auto size = M_texture_size / gtp;

            auto framebuffer = gl::Framebuffer();
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            M_object_texture = gl::Texture();
            glBindTexture(GL_TEXTURE_2D, M_object_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
                         M_texture_size, M_texture_size, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_2D, M_object_texture, 0);
            auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if (status != GL_FRAMEBUFFER_COMPLETE) {
                throw std::runtime_error("Error: Framebuffer is not complete "
                        "when performing a texture transform.");
            }
            glViewport(0, 0, M_texture_size, M_texture_size);
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);

            auto fake_camera = Camera(20, size, -size);
            fake_camera.shift((x1 + x2)/2*e1 + (y1 + y2)/2*e2);

            bool is_visible = tracked_object().is_visible();
            if (!is_visible) tracked_object().set_visible(true);
            tracked_object().draw(fake_camera);
            if (!is_visible) tracked_object().set_visible(false);

            M_distance_transform = distance_transform(
                alpha_threshold(
                    M_object_texture,
                    0.01,
                    M_texture_size,
                    M_texture_size
                ),
                M_texture_size,
                1.0/M_texture_size
            );

            glBindTexture(GL_TEXTURE_2D, M_distance_transform);
            glTexParameteri(
                GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR
            );
            glTexParameteri(
                GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR
            );
            glGenerateMipmap(GL_TEXTURE_2D);

            M_x1 = x1;
            M_x2 = x2;
            M_y1 = y1;
            M_y2 = y2;

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, current_draw_framebuffer);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, current_read_framebuffer);
            glViewport(
                current_viewport[0], current_viewport[1],
                current_viewport[2], current_viewport[3]
            );
        }
    };
    struct TransformingPart : public SingleObject {
        TransformingPart()
        {
            glBindVertexArray(M_vertex_array);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_element_buffer);
            auto indices = std::array<unsigned, 6>{{
                0, 1, 2, 2, 1, 3
            }};
            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                sizeof(unsigned)*6,
                indices.data(),
                GL_STATIC_DRAW
            );
            glBindVertexArray(0);
        }
        std::unique_ptr<StaticPart> anim_copy() const
        {
            return std::make_unique<StaticPart>();
        }
        using SingleObject::interpolate;
        void interpolate(
            const StaticPart&,
            const StaticPart&,
            double t
        )
        {
            M_t = t;
            auto& from = **M_from->M_tracked_object;
            auto& to = **M_to->M_tracked_object;
            interpolate(from, to, t);
            reset();
            auto c1 = from.get_outline_color();
            auto c2 = to.get_outline_color();
            auto t1 = from.get_outline_thickness();
            auto t2 = from.get_outline_thickness();
            auto outline_color = Color(
                static_cast<unsigned char>(c1.r * (1 - t) + c2.r * t),
                static_cast<unsigned char>(c1.g * (1 - t) + c2.g * t),
                static_cast<unsigned char>(c1.b * (1 - t) + c2.b * t),
                static_cast<unsigned char>(c1.a * (1 - t) + c2.a * t)
            );
            set_outline(outline_color, t1 * (1 - t) + t2 * t);
        }

        virtual void draw(const Camera& camera) override
        {
            auto [from_object, from_distance_transform]
                = M_from->get_textures(camera);
            auto [to_object, to_distance_transform]
                = M_to->get_textures(camera);

            if (M_scale1 == 0) {
                get_scales(from_distance_transform, to_distance_transform);
            }

            float x1 = M_from->M_x1 * (1 - M_t) + M_to->M_x1 * M_t;
            float x2 = M_from->M_x2 * (1 - M_t) + M_to->M_x2 * M_t;
            float y1 = M_from->M_y1 * (1 - M_t) + M_to->M_y1 * M_t;
            float y2 = M_from->M_y2 * (1 - M_t) + M_to->M_y2 * M_t;
            float t1x1 = (1 - double(M_from->M_texture_width)
                    / M_from->M_texture_size)/2;
            float t1x2 = (1 + double(M_from->M_texture_width)
                    / M_from->M_texture_size)/2;
            float t1y1 = (1 - double(M_from->M_texture_height)
                    / M_from->M_texture_size)/2;
            float t1y2 = (1 + double(M_from->M_texture_height)
                    / M_from->M_texture_size)/2;
            float t2x1 = (1 - double(M_to  ->M_texture_width)
                    / M_to  ->M_texture_size)/2;
            float t2x2 = (1 + double(M_to  ->M_texture_width)
                    / M_to  ->M_texture_size)/2;
            float t2y1 = (1 - double(M_to  ->M_texture_height)
                    / M_to  ->M_texture_size)/2;
            float t2y2 = (1 + double(M_to  ->M_texture_height)
                    / M_to  ->M_texture_size)/2;
            M_vertices = {{
                {x1, y2, t1x1, t1y2, t2x1, t2y2},
                {x2, y2, t1x2, t1y2, t2x2, t2y2},
                {x1, y1, t1x1, t1y1, t2x1, t2y1},
                {x2, y1, t1x2, t1y1, t2x2, t2y1}
            }};

            auto& shader = get_shader();
            glUseProgram(shader);
            glUniform2f(shader.get_uniform("camera_scale"),
                        camera.get_x_scale(), camera.get_y_scale());
            shader.set_rotor_uniform("view", ~camera.get_rotor());
            shader.set_rotor_uniform("model", get_rotor());

            glBindVertexArray(M_vertex_array);

            glBindBuffer(GL_ARRAY_BUFFER, M_vertex_buffer);
            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(Vertex)*4,
                M_vertices.data(),
                GL_STREAM_DRAW
            );
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  reinterpret_cast<void*>(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  reinterpret_cast<void*>(2*sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  reinterpret_cast<void*>(4*sizeof(float)));
            glEnableVertexAttribArray(2);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, from_object);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, from_distance_transform);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, to_object);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, to_distance_transform);
            glUniform1i(shader.get_uniform("object1"), 0);
            glUniform1i(shader.get_uniform("distance_transform1"), 1);
            glUniform1i(shader.get_uniform("object2"), 2);
            glUniform1i(shader.get_uniform("distance_transform2"), 3);
            glUniform1f(shader.get_uniform("t"), M_t);
            glUniform1f(shader.get_uniform("scale1"), M_scale1);
            glUniform1f(shader.get_uniform("scale2"), M_scale2);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
        virtual void draw_outline(const Camera& camera) override
        {
            M_from->get_textures(camera);
            M_to->get_textures(camera);
            auto this_scale = get_scale();
            scale(1/this_scale);
            SingleObject::draw_outline(camera);
            scale(this_scale);
        }
        void get_scales(unsigned from, unsigned to)
        {
            const auto from_size = M_from->M_texture_size;
            const auto to_size = M_to->M_texture_size;

            auto from_values = std::vector<float>();
            from_values.resize(from_size * from_size);
            glGetTextureImage(
                from, 0, GL_RED, GL_FLOAT,
                from_size * from_size * sizeof(float), from_values.data()
            );

            auto to_values = std::vector<float>();
            to_values.resize(to_size * to_size);
            glGetTextureImage(
                to, 0, GL_RED, GL_FLOAT,
                to_size * to_size * sizeof(float), to_values.data()
            );

            const auto from_width = M_from->M_texture_width;
            const auto from_height = M_from->M_texture_height;
            const auto from_x_plus = (from_size - from_width) / 2;
            const auto from_y_plus = (from_size - from_height) / 2;
            const auto to_width = M_to->M_texture_width;
            const auto to_height = M_to->M_texture_height;
            const auto to_x_plus = (to_size - to_width) / 2;
            const auto to_y_plus = (to_size - to_height) / 2;
            for (int x = 0; x < from_width; ++x) {
                for (int y = 0; y < from_height; ++y) {
                    int x1 = std::min(x + from_x_plus, from_size - 1);
                    int y1 = std::min(y + from_y_plus, from_size - 1);
                    if (from_values[x1 + y1*from_size] == 0) {
                        int x2 = std::min(
                            int(double(x)/from_width * to_width) + to_x_plus,
                            to_size - 1
                        );
                        int y2 = std::min(
                            int(double(y)/from_height * to_height) + to_y_plus,
                            to_size - 1
                        );
                        M_scale1 = std::max(
                            M_scale1,
                            to_values[x2 + y2*to_size]
                        );
                    }
                }
            }
            for (int x = 0; x < to_width; ++x) {
                for (int y = 0; y < to_height; ++y) {
                    int x1 = std::min(x + to_x_plus, to_size - 1);
                    int y1 = std::min(y + to_y_plus, to_size - 1);
                    if (to_values[x1 + y1*to_size] == 0) {
                        int x2 = std::min(
                            int(double(x)/to_width * from_width) + from_x_plus,
                            to_size - 1
                        );
                        int y2 = std::min(
                            int(double(y)/to_height * from_height)+ from_y_plus,
                            to_size - 1
                        );
                        M_scale2 = std::max(
                            M_scale2,
                            from_values[x2 + y2*from_size]
                        );
                    }
                }
            }
            if (M_scale1 == 0) M_scale1 = 1;
            if (M_scale2 == 0) M_scale2 = 1;
        }
        virtual Box get_original_true_bounding_box() const override
        {
            const auto& box1 = M_from->M_bounding_box;
            const auto& box2 = M_to->M_bounding_box;
            auto p11 = box1.get_inner_lower_left_vertex().undual();
            auto p12 = box1.get_outer_upper_right_vertex().undual();
            auto p21 = box2.get_inner_lower_left_vertex().undual();
            auto p22 = box2.get_outer_upper_right_vertex().undual();
            auto p1 = p11 * (1 - M_t) + p21 * M_t;
            auto p2 = p12 * (1 - M_t) + p22 * M_t;
            auto rotor = get_rotor();
            auto convert_point = [&](const vga3::Vec& p) {
                auto new_p = ~rotor * vga3_to_pga3(get_scale()*p) * rotor;
                return pga3_to_vga3(new_p.grade_project<3>());
            };
            auto points = std::array<vga3::Vec, 8>();
            int i = 0;
            {
                using namespace pga3;
                for (auto x : {p1.blade_project<e1>(),
                               p2.blade_project<e1>()}) {
                    for (auto y : {p1.blade_project<e2>(),
                                   p2.blade_project<e2>()}) {
                        for (auto z : {p1.blade_project<e3>(),
                                       p2.blade_project<e3>()}) {
                            points[i++] = {x, y, z};
                        }
                    }
                }
            }
            using namespace vga3;
            auto transformed_points = points | std::views::transform(convert_point);
            auto xs = transformed_points
                | std::views::transform(&vga3::Vec::blade_project<e1>);
            auto ys = transformed_points
                | std::views::transform(&vga3::Vec::blade_project<e2>);
            auto zs = transformed_points
                | std::views::transform(&vga3::Vec::blade_project<e3>);
            return Box(
                vga3::Vec{
                    *std::ranges::min_element(xs),
                    *std::ranges::min_element(ys),
                    *std::ranges::min_element(zs)
                },
                vga3::Vec{
                    *std::ranges::max_element(xs),
                    *std::ranges::max_element(ys),
                    *std::ranges::max_element(zs)
                }
            );
        }

        StaticPart* M_from = nullptr;
        StaticPart* M_to = nullptr;
        double M_t = 0;
        struct Vertex {
            float x;
            float y;
            float t1x;
            float t1y;
            float t2x;
            float t2y;
        };
        std::array<Vertex, 4> M_vertices;
        gl::VertexArray M_vertex_array;
        gl::Buffer M_vertex_buffer;
        gl::Buffer M_element_buffer;
        float M_scale1 = 0;
        float M_scale2 = 0;
    };
}

void ganim::texture_transform(
    SceneBase& scene,
    MaybeOwningRef<Object> from,
    MaybeOwningRef<Object> to,
    TransformAnimationArgs args
)
{
    if (from->is_animating()) {
        throw std::invalid_argument(
            "Attempting to animate an object that is already being animated."
        );
    }
    to->set_animating(true);
    if (!args.copy) from->set_visible(false);
    auto temp_object = std::make_unique<TransformingPart>();

    auto anim = Animation(
        scene,
        MaybeOwningRef(*temp_object),
        {args.duration, args.rate_function}
    );
    auto& object = *temp_object;
    anim.add_animation_object(std::move(temp_object));
    auto& from_part = anim.get_starting_object();
    auto& to_part = anim.get_ending_object();
    from_part.M_tracked_object = std::move(from);
    to_part.M_tracked_object = std::move(to);
    object.M_from = &from_part;
    object.M_to = &to_part;

    anim.at_end([&to = *to]{
        to.set_visible(true);
        to.set_animating(false);
    });
    object.add_updater(std::move(anim), true);
    object.add_updater([&object = object, direction = args.direction]{
        object.shift(std::sin(object.M_t * τ/2)*direction);
    });
}

void ganim::group_transform(
    SceneBase& scene,
    MaybeOwningRef<Group> from,
    MaybeOwningRef<Group> to,
    TransformAnimationArgs args
)
{
    auto indices = discrete_interpolate(from->size(), to->size());
    for (int i = 0; i < ssize(indices); ++i) {
        for (auto j : indices[i]) {
            texture_transform(scene, (*from)[i], (*to)[j], args);
        }
    }
}
