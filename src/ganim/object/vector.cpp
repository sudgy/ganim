#include "vector.hpp"

#include "ganim/math.hpp"

using namespace ganim;

Vector::Vector(pga3::Trivector p1, pga3::Trivector p2, VectorArgs args)
:   M_max_tip_to_length_ratio(args.max_tip_to_length_ratio),
    M_tip_size(args.tip_size)
{
    set_start_and_end(p1, p2);
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
        apply_rotor(current_line * (current_line + final_line).normalized());
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

Vector& Vector::scale(const pga3::Trivector& about_point, double scale)
{
    if (M_manual_transform) {
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
    if (!M_manual_transform) {
        using namespace pga3;
        auto start_point = get_start_pga3();
        auto end_point = get_end_pga3();
        auto vec_line = (start_point & end_point).normalized();
        auto good_plane = -(e3 | vec_line) | vec_line;
        if (good_plane.norm2() < 1e-5) return *this;
        else good_plane = good_plane.normalized();

        auto r = get_rotor();
        auto bad_plane = (~r*e3*r).grade_project<1>();
        M_manual_transform = true;
        apply_rotor(bad_plane * (bad_plane + good_plane).normalized());
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
    (void)camera; // For now
}

/*
void Vector::calculate_size()
{
    auto length = (M_end - M_start).norm();
    // Length is long enough, use the default tip size
    if (length * M_max_tip_to_length_ratio > M_tip_size) {
        M_tip_pos = length - M_tip_size;
    }
    // Length is not long enough, start squishing the tip
    else {
        M_tip_pos = length * (1 - M_max_tip_to_length_ratio);
    }
}
*/
