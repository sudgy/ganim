#include "vector.hpp"

#include "ganim/ga/exp.hpp"
#include "ganim/math.hpp"

#include "ganim/gl/gl.hpp"

#include "shaders.hpp"

using namespace ganim;

Vector::Vector(pga3::Trivector p1, pga3::Trivector p2, VectorArgs args)
:   M_max_tip_to_length_ratio(args.max_tip_to_length_ratio),
    M_tip_size(args.tip_size),
    M_do_shading(args.three_d)
{
    set_start_and_end(p1, p2);
    glBindVertexArray(M_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, M_vertex_buffer);
    float t = args.thickness / 2;
    float y = M_tip_size / 2;
    auto vertices = std::array{
        0.0f,   t,  t, 0.0f,
        0.0f,  -t,  t, 0.0f,
        0.5f,   t,  t, 0.5f,
        0.5f,  -t,  t, 0.5f,
        0.5f,   y,  t, 0.5f,
        0.5f,  -y,  t, 0.5f,
        1.0f, 0.f,  t, 1.0f,
        0.0f,   t, -t, 0.0f,
        0.0f,  -t, -t, 0.0f,
        0.5f,   t, -t, 0.5f,
        0.5f,  -t, -t, 0.5f,
        0.5f,   y, -t, 0.5f,
        0.5f,  -y, -t, 0.5f,
        1.0f, 0.f, -t, 1.0f
    };
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices.data(),
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 4*sizeof(float),
        reinterpret_cast<void*>(0)
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1, 1, GL_FLOAT, GL_FALSE, 4*sizeof(float),
        reinterpret_cast<void*>(3*sizeof(float))
    );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_element_buffer);
    auto indices = std::array{
         0,  1,  2,
         2,  1,  3,
         4,  5,  6,

         0,  7,  1,
         7,  8,  1,
         0,  2,  7,
         2,  9,  7,
         1,  8,  3,
         3,  8, 10,
         2,  4,  9,
         9,  4, 11,
         3, 10,  5,
         5, 10, 12,
         4,  6, 11,
        11,  6, 13,
         6,  5, 12,
         6, 12, 13,

         7,  9,  8,
         9, 10,  8,
        11, 13, 12
    };
    if (args.three_d) {
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(indices),
            indices.data(),
            GL_STATIC_DRAW
        );
    }
    else {
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(unsigned)*9,
            indices.data(),
            GL_STATIC_DRAW
        );
    }
    glBindVertexArray(0);
}

Vector& Vector::set_start(pga3::Trivector p)
{
    set_start_and_end(p, get_end_pga3());
    return *this;
}

Vector& Vector::set_end(pga3::Trivector p)
{
    using namespace pga3;
    p /= p.blade_project<e123>();
    const auto current_start = get_start_pga3();
    auto current_end = get_end_pga3();
    const auto current_length = (current_end - current_start).undual().norm();
    const auto new_length = (p - current_start).undual().norm();
    M_manual_transform = true;
    if (current_length == 0) {
        reset_scale();
        scale(new_length);
    }
    else {
        scale(new_length / current_length);
    }
    M_manual_transform = false;
    if (new_length != 0) {
        current_end = get_end_pga3();
        auto current_line = (current_start & current_end).normalized();
        auto final_line = (current_start & p).normalized();
        auto mid_line = current_line + final_line;
        if (mid_line.norm2() < 1e-10) {
            auto axis_plane = current_line | current_start;
            auto axis = (e12 | axis_plane) ^ axis_plane;
            if (axis.norm2() < 1e-10) {
                axis = (e13 | axis_plane) ^ axis_plane;
            }
            axis = axis.normalized();
            apply_rotor(ga_exp(axis * τ/4));
        }
        else {
            apply_rotor(-current_line * mid_line.normalized());
        }
    }
    return *this;
}

Vector& Vector::set_start_and_end(pga3::Trivector p1, pga3::Trivector p2)
{
    using namespace pga3;
    auto current_start = get_start_pga3();
    p1 /= p1.blade_project<e123>();
    M_manual_transform = true;
    shift(p1 - current_start + e123);
    set_end(p2);
    return *this;
}

pga2::Bivector Vector::get_start_pga2() const
{
    return pga3_to_pga2(get_start_pga3());
}

pga3::Trivector Vector::get_start_pga3() const
{
    using namespace pga3;
    const auto r = get_rotor();
    return (~r*e123*r).grade_project<3>();
}

vga2::Vector Vector::get_start_vga2() const
{
    return vga3_to_vga2(get_start_vga3());
}

vga3::Vector Vector::get_start_vga3() const
{
    return pga3_to_vga3(get_start_pga3());
}

pga2::Bivector Vector::get_end_pga2() const
{
    return pga3_to_pga2(get_end_pga3());
}

pga3::Trivector Vector::get_end_pga3() const
{
    using namespace pga3;
    const auto r = get_rotor();
    return (~r*(e0 + get_scale()*e1).dual()*r).grade_project<3>();
}

vga2::Vector Vector::get_end_vga2() const
{
    return vga3_to_vga2(get_end_vga3());
}

vga3::Vector Vector::get_end_vga3() const
{
    return pga3_to_vga3(get_end_pga3());
}

void Vector::lock_orientation(bool lock)
{
    M_lock_orientation = lock;
    if (lock) apply_rotor(pga3::Even(1));
}

Vector& Vector::scale(const pga3::Trivector& about_point, double scale)
{
    if (M_manual_transform or M_animating) {
        Object::scale(about_point, scale);
        return *this;
    }
    using namespace pga3;
    auto about_point2 = about_point;
    about_point2 /= about_point2.blade_project<e123>();
    auto transform = [&](pga3::Trivector point) {
        point -= about_point2 - e123;
        point *= scale;
        point += (1 - scale)*e123;
        point += about_point - e123;
        return point;
    };
    auto new_start = transform(get_start_pga3());
    auto new_end = transform(get_end_pga3());
    set_start_and_end(new_start, new_end);
    return *this;
}

Vector& Vector::apply_rotor(const pga3::Even& rotor)
{
    Object::apply_rotor(rotor);
    if (!M_manual_transform and M_lock_orientation) {
        using namespace pga3;
        auto start_point = get_start_pga3();
        auto end_point = get_end_pga3();
        auto vec_line = start_point & end_point;
        if (vec_line.norm2() < 1e-10) return *this;
        else vec_line = vec_line.normalized();
        auto good_plane = -(e3 | vec_line) | vec_line;
        if (good_plane.norm2() < 1e-10) return *this;
        else good_plane = good_plane.normalized();

        auto r = get_rotor();
        auto bad_plane = (~r*e3*r).grade_project<1>();
        M_manual_transform = true;
        auto mid_plane = bad_plane + good_plane;
        if (mid_plane.norm2() < 1e-10) {
            apply_rotor(ga_exp(vec_line*τ/4));
        }
        else {
            apply_rotor(bad_plane * mid_plane.normalized());
        }
        M_manual_transform = false;
    }
    return *this;
}

Box Vector::get_original_true_bounding_box() const
{
    auto y = M_tip_size / 2.0;
    using namespace vga2;
    return {-y*e2, get_scale()*e1 + y*e2};
}

void Vector::draw(const Camera& camera)
{
    auto& shader = *get_shader();
    glUseProgram(shader);
    glUniform2f(shader.get_uniform("camera_scale"),
                camera.get_x_scale(), camera.get_y_scale());
    shader.set_rotor_uniform("view", ~camera.get_rotor());
    shader.set_rotor_uniform("model", get_rotor());
    glUniform4f(shader.get_uniform("object_color"),
            get_color().r / 255.0, get_color().g / 255.0,
            get_color().b / 255.0, get_color().a / 255.0);
    glUniform1f(shader.get_uniform("scale"), 1);
    if (is_creating()) {
        glUniform1f(shader.get_uniform("this_t"), get_draw_fraction());
    }
    else if (auto noise = noise_creating()) {
        auto actual_draw_fraction =
            -noise + (1 + 2*noise) * get_draw_fraction();
        glUniform1f(shader.get_uniform("this_t"), actual_draw_fraction);
        glUniform1f(shader.get_uniform("noise_scale"), noise);
    }

    auto length = (get_end_vga3() - get_start_vga3()).norm();
    glUniform1f(shader.get_uniform("end_pos"), length);
    // Length is long enough, use the default tip size
    if (length * M_max_tip_to_length_ratio > M_tip_size) {
        glUniform1f(shader.get_uniform("mid_pos"), length - M_tip_size);
    }
    // Length is not long enough, start squishing the tip
    else {
        glUniform1f(
            shader.get_uniform("mid_pos"),
            length * (1 - M_max_tip_to_length_ratio)
        );
    }

    glBindVertexArray(M_vertex_array);
    glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

gl::Shader* Vector::get_shader()
{
    using enum ShaderFeature;
    auto flags = Time | Vector;
    if (M_do_shading) flags |= FaceShading;
    if (is_creating()) flags |= Create;
    else if (noise_creating()) flags |= NoiseCreate;
    return &ganim::get_shader(flags);
}

std::unique_ptr<Vector> Vector::anim_copy() const
{
    // Calling new directly here because make_unique apparently can't call
    // private constructors
    return std::unique_ptr<Vector>(new Vector(*this));
}

Vector::Vector(const Vector& other)
:   M_vertex_array(0),
    M_vertex_buffer(0),
    M_element_buffer(0),
    M_max_tip_to_length_ratio(other.M_max_tip_to_length_ratio),
    M_tip_size(other.M_tip_size),
    M_manual_transform(other.M_manual_transform),
    M_animating(true)
{
    interpolate(other, other, 0.0);
}

void Vector::interpolate(const Vector& start, const Vector& end, double t)
{
    SingleObject::interpolate(start, end, t);
    if (t == 1.0) set_start_and_end(end.get_start_pga3(), end.get_end_pga3());
}
