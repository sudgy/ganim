#include "vector.hpp"

#include "ganim/math.hpp"

using namespace ganim;

Vector::Vector(pga3::Trivector p1, pga3::Trivector p2, VectorArgs args)
:   M_max_tip_to_length_ratio(args.max_tip_to_length_ratio),
    M_tip_size(args.tip_size),
    M_start(pga3_to_vga3(p1)),
    M_end(pga3_to_vga3(p2))
{
    reposition();
}

Vector& Vector::set_start(pga3::Trivector p)
{
    M_start = pga3_to_vga3(p);
    reposition();
    return *this;
}

Vector& Vector::set_end(pga3::Trivector p)
{
    M_end = pga3_to_vga3(p);
    reposition();
    return *this;
}

Vector& Vector::set_start_and_end(pga3::Trivector p1, pga3::Trivector p2)
{
    M_start = pga3_to_vga3(p1);
    M_end = pga3_to_vga3(p2);
    reposition();
    return *this;
}

pga2::Bivector Vector::get_start_pga2() const
{
    return vga2_to_pga2(get_start_vga2());
}

pga3::Trivector Vector::get_start_pga3() const
{
    return vga3_to_pga3(M_start);
}

vga2::Vector Vector::get_start_vga2() const
{
    return vga3_to_vga2(M_start);
}

vga3::Vector Vector::get_start_vga3() const
{
    return M_start;
}

pga2::Bivector Vector::get_end_pga2() const
{
    return vga2_to_pga2(get_end_vga2());
}

pga3::Trivector Vector::get_end_pga3() const
{
    return vga3_to_pga3(M_end);
}

vga2::Vector Vector::get_end_vga2() const
{
    return vga3_to_vga2(M_end);
}

vga3::Vector Vector::get_end_vga3() const
{
    return M_end;
}

Vector& Vector::scale(const pga3::Trivector& about_point, double scale)
{
    auto vga_about = pga3_to_vga3(about_point);
    auto transform = [&](vga3::Vector point) {
        point -= vga_about;
        point *= scale;
        point += vga_about;
        return point;
    };
    M_start = transform(M_start);
    M_end = transform(M_end);
    reposition();
    return *this;
}

Vector& Vector::apply_rotor(const pga3::Even& rotor)
{
    Object::apply_rotor(rotor);
    if (!M_redoing) {
        auto transform = [&](auto p){
            auto new_p = ~rotor * vga3_to_pga3(p) * rotor;
            return pga3_to_vga3(new_p.template grade_project<3>());
        };
        M_start = transform(M_start);
        M_end = transform(M_end);
        reorient();
    }
    return *this;
}

Box Vector::get_original_true_bounding_box() const
{
    auto length = (M_end - M_start).norm();
    auto y = M_tip_size / 2.0;
    using namespace vga2;
    return {-y*e2, length*e1 + y*e2};
}

void Vector::draw(const Camera& camera)
{
    (void)camera; // For now
}

void Vector::reposition()
{
    M_redoing = true;
    reset();
    using namespace vga3;
    auto dir = M_end - M_start;
    auto proj = (dir | e12) | e21;
    // Vector pointing straight up, just pick any random orientation
    if (proj == 0) {
        Object::rotate(e13, std::copysign(τ/4, dir.blade_project<e3>()));
    }
    // Vector not pointing straight up
    else {
        reset();
        auto v1 = e1;
        auto v2 = proj.normalized();
        auto v3 = dir.normalized();
        apply_rotor(v1 * (v1 + v2).normalized());
        apply_rotor(v2 * (v2 + v3).normalized());
    }
    shift(M_start);
    calculate_size();
    M_redoing = false;
}

void Vector::reorient()
{
    using namespace pga3;
    auto rotor = get_rotor();
    auto start_point = vga3_to_pga3(M_start);
    auto end_point = vga3_to_pga3(M_end);
    auto vec_line = (start_point & end_point).normalized();
    auto good_plane = -(e3 | vec_line) | vec_line;
    if (good_plane.norm2() < 1e-5) return;
    else good_plane = good_plane.normalized();

    auto bad_point = ~rotor * (e2 + e0).dual() * rotor;
    auto bad_plane = (start_point & end_point & bad_point).normalized();
    M_redoing = true;
    apply_rotor(bad_plane * (bad_plane + good_plane).normalized());
    M_redoing = false;
}

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
