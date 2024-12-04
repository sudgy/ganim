#include "box.hpp"

#include <ranges>

#include "ganim/ga/conversions.hpp"

using namespace ganim;
using namespace pga3;

Box::Box() : p1(e123), p2(e123) {}

Box::Box(const pga3::Trivector& p1, const pga3::Trivector& p2)
    : p1(p1), p2(p2)
{
    this->p1 /= p1.blade_project<e123>();
    this->p2 /= p2.blade_project<e123>();
}

pga3::Vector Box::get_left_face() const
{
    return (e1 | ~p1) | p1;
}

pga3::Vector Box::get_right_face() const
{
    return (e1 | ~p2) | p2;
}

pga3::Vector Box::get_up_face() const
{
    return (e2 | ~p2) | p2;
}

pga3::Vector Box::get_down_face() const
{
    return (e2 | ~p1) | p1;
}

pga3::Vector Box::get_out_face() const
{
    return (e3 | ~p2) | p2;
}

pga3::Vector Box::get_in_face() const
{
    return (e3 | ~p1) | p1;
}

pga3::Bivector Box::get_upper_left_edge() const
{
    return get_left_face() ^ get_up_face();
}

pga3::Bivector Box::get_upper_right_edge() const
{
    return get_right_face() ^ get_up_face();
}

pga3::Bivector Box::get_lower_left_edge() const
{
    return get_left_face() ^ get_down_face();
}

pga3::Bivector Box::get_lower_right_edge() const
{
    return get_right_face() ^ get_down_face();
}

pga3::Bivector Box::get_outer_left_edge() const
{
    return get_left_face() ^ get_out_face();
}

pga3::Bivector Box::get_outer_right_edge() const
{
    return get_right_face() ^ get_out_face();
}

pga3::Bivector Box::get_outer_up_edge() const
{
    return get_up_face() ^ get_out_face();
}

pga3::Bivector Box::get_outer_down_edge() const
{
    return get_down_face() ^ get_out_face();
}

pga3::Bivector Box::get_inner_left_edge() const
{
    return get_left_face() ^ get_in_face();
}

pga3::Bivector Box::get_inner_right_edge() const
{
    return get_right_face() ^ get_in_face();
}

pga3::Bivector Box::get_inner_up_edge() const
{
    return get_up_face() ^ get_in_face();
}

pga3::Bivector Box::get_inner_down_edge() const
{
    return get_down_face() ^ get_in_face();
}

pga3::Trivector Box::get_outer_upper_left_vertex() const
{
    return get_left_face() ^ get_up_face() ^ get_out_face();
}

pga3::Trivector Box::get_outer_upper_right_vertex() const
{
    return p2;
}

pga3::Trivector Box::get_outer_lower_left_vertex() const
{
    return get_left_face() ^ get_down_face() ^ get_out_face();
}

pga3::Trivector Box::get_outer_lower_right_vertex() const
{
    return get_right_face() ^ get_down_face() ^ get_out_face();
}

pga3::Trivector Box::get_inner_upper_left_vertex() const
{
    return get_left_face() ^ get_up_face() ^ get_in_face();
}

pga3::Trivector Box::get_inner_upper_right_vertex() const
{
    return get_right_face() ^ get_up_face() ^ get_in_face();
}

pga3::Trivector Box::get_inner_lower_left_vertex() const
{
    return p1;
}

pga3::Trivector Box::get_inner_lower_right_vertex() const
{
    return get_right_face() ^ get_down_face() ^ get_in_face();
}

pga3::Trivector Box::get_center_3d() const
{
    return (p1 + p2) / 2;
}

pga3::Trivector Box::get_left_3d() const
{
    auto face = get_left_face();
    auto center = get_center_3d();
    return (face | center) ^ face;
}

pga3::Trivector Box::get_right_3d() const
{
    auto face = get_right_face();
    auto center = get_center_3d();
    return (face | center) ^ face;
}

pga3::Trivector Box::get_up_3d() const
{
    auto face = get_up_face();
    auto center = get_center_3d();
    return (face | center) ^ face;
}

pga3::Trivector Box::get_down_3d() const
{
    auto face = get_down_face();
    auto center = get_center_3d();
    return (face | center) ^ face;
}

pga3::Trivector Box::get_out_3d() const
{
    auto face = get_out_face();
    auto center = get_center_3d();
    return (face | center) ^ face;
}

pga3::Trivector Box::get_in_3d() const
{
    auto face = get_in_face();
    auto center = get_center_3d();
    return (face | center) ^ face;
}

pga2::Vector Box::get_left_edge() const
{
    return pga3_to_pga2(get_left_face());
}

pga2::Vector Box::get_right_edge() const
{
    return pga3_to_pga2(get_right_face());
}

pga2::Vector Box::get_up_edge() const
{
    return pga3_to_pga2(get_up_face());
}

pga2::Vector Box::get_down_edge() const
{
    return pga3_to_pga2(get_down_face());
}

pga2::Bivector Box::get_center() const
{
    return pga3_to_pga2(get_center_3d());
}

pga2::Bivector Box::get_left() const
{
    auto edge = get_left_edge();
    auto center = get_center();
    return (center | edge) ^ edge;
}

pga2::Bivector Box::get_right() const
{
    auto edge = get_right_edge();
    auto center = get_center();
    return (center | edge) ^ edge;
}

pga2::Bivector Box::get_up() const
{
    auto edge = get_up_edge();
    auto center = get_center();
    return (center | edge) ^ edge;
}

pga2::Bivector Box::get_down() const
{
    auto edge = get_down_edge();
    auto center = get_center();
    return (center | edge) ^ edge;
}

pga2::Bivector Box::get_upper_left() const
{
    return get_left_edge() ^ get_up_edge();
}

pga2::Bivector Box::get_upper_right() const
{
    return get_right_edge() ^ get_up_edge();
}

pga2::Bivector Box::get_lower_left() const
{
    return get_left_edge() ^ get_down_edge();
}

pga2::Bivector Box::get_lower_right() const
{
    return get_right_edge() ^ get_down_edge();
}

pga2::Bivector Box::get_outside_point(const pga2::Bivector& point) const
{
    using namespace pga2;
    auto p = point;
    p -= p.blade_project<e12>() * e12;
    auto center = get_center();
    auto line = center & p;
    auto result = Bivector();
    auto distance = INFINITY;
    for (auto side : {
            -get_left_edge(),
            get_right_edge(),
            get_up_edge(),
            -get_down_edge()
        })
    {
        auto intersect = side ^ line;
        if (intersect.blade_project<e12>() > 0) {
            intersect /= intersect.blade_project<e12>();
            auto this_distance = (center - intersect).undual().norm2();
            if (this_distance < distance) {
                result = intersect;
                distance = this_distance;
            }
        }
    }
    return result;
}

pga3::Trivector Box::get_outside_point_3d(const pga3::Trivector& point) const
{
    using namespace pga3;
    auto p = point;
    p -= p.blade_project<e123>() * e123;
    auto center = get_center_3d();
    auto line = center & p;
    auto result = Trivector();
    auto distance = INFINITY;
    for (auto side : {
            -get_left_face(),
            get_right_face(),
            get_up_face(),
            -get_down_face(),
            get_out_face(),
            -get_in_face()
        })
    {
        auto intersect = side ^ line;
        if (intersect.blade_project<e123>() > 0) {
            intersect /= intersect.blade_project<e123>();
            auto this_distance = (center - intersect).undual().norm2();
            if (this_distance < distance) {
                result = intersect;
                distance = this_distance;
            }
        }
    }
    return result;
}

double Box::get_width() const
{
    return (p2 - p1).undual().blade_project<e1>();
}

double Box::get_height() const
{
    return (p2 - p1).undual().blade_project<e2>();
}

double Box::get_thickness() const
{
    return (p2 - p1).undual().blade_project<e3>();
}

Box ganim::merge_boxes(const Box& box1, const Box& box2)
{
    auto p11 = box1.get_inner_lower_left_vertex().undual();
    auto p12 = box1.get_outer_upper_right_vertex().undual();
    auto p21 = box2.get_inner_lower_left_vertex().undual();
    auto p22 = box2.get_outer_upper_right_vertex().undual();
    auto p1x = std::min(p11.blade_project<e1>(), p21.blade_project<e1>());
    auto p1y = std::min(p11.blade_project<e2>(), p21.blade_project<e2>());
    auto p1z = std::min(p11.blade_project<e3>(), p21.blade_project<e3>());
    auto p2x = std::max(p12.blade_project<e1>(), p22.blade_project<e1>());
    auto p2y = std::max(p12.blade_project<e2>(), p22.blade_project<e2>());
    auto p2z = std::max(p12.blade_project<e3>(), p22.blade_project<e3>());
    return Box(
        !(p1x*e1 + p1y*e2 + p1z*e3 + e0),
        !(p2x*e1 + p2y*e2 + p2z*e3 + e0)
    );
}

Box ganim::transform_box(const Box& box, const pga3::Even& rotor, double scale)
{
    auto convert_point = [&](vga3::Vector p) {
        auto new_p = ~rotor * vga3_to_pga3(scale*p) * rotor;
        return pga3_to_vga3(new_p.grade_project<3>());
    };
    auto points = std::array<vga3::Vector, 8>();
    int i = 0;
    auto p1 = box.get_inner_lower_left_vertex().undual();
    auto p2 = box.get_outer_upper_right_vertex().undual();
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
    auto transformed_points = points | std::views::transform(convert_point);
    auto xs = transformed_points
        | std::views::transform(&vga3::Vector::blade_project<vga3::e1>);
    auto ys = transformed_points
        | std::views::transform(&vga3::Vector::blade_project<vga3::e2>);
    auto zs = transformed_points
        | std::views::transform(&vga3::Vector::blade_project<vga3::e3>);
    return Box(
        vga3::Vector{
            *std::ranges::min_element(xs),
            *std::ranges::min_element(ys),
            *std::ranges::min_element(zs)
        },
        vga3::Vector{
            *std::ranges::max_element(xs),
            *std::ranges::max_element(ys),
            *std::ranges::max_element(zs)
        }
    );
}
