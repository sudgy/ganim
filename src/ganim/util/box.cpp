#include "box.hpp"

#include <ranges>

#include "ganim/ga/conversions.hpp"

using namespace ganim;
using namespace pga3;

pga3::Vec BoxInterface::get_left_face() const
{
    auto [p1, p2] = get_box();
    return (e1 | ~p1) | p1;
}

pga3::Vec BoxInterface::get_right_face() const
{
    auto [p1, p2] = get_box();
    return (e1 | ~p2) | p2;
}

pga3::Vec BoxInterface::get_up_face() const
{
    auto [p1, p2] = get_box();
    return (e2 | ~p2) | p2;
}

pga3::Vec BoxInterface::get_down_face() const
{
    auto [p1, p2] = get_box();
    return (e2 | ~p1) | p1;
}

pga3::Vec BoxInterface::get_out_face() const
{
    auto [p1, p2] = get_box();
    return (e3 | ~p2) | p2;
}

pga3::Vec BoxInterface::get_in_face() const
{
    auto [p1, p2] = get_box();
    return (e3 | ~p1) | p1;
}

pga3::Bivec BoxInterface::get_upper_left_edge() const
{
    return get_left_face() ^ get_up_face();
}

pga3::Bivec BoxInterface::get_upper_right_edge() const
{
    return get_right_face() ^ get_up_face();
}

pga3::Bivec BoxInterface::get_lower_left_edge() const
{
    return get_left_face() ^ get_down_face();
}

pga3::Bivec BoxInterface::get_lower_right_edge() const
{
    return get_right_face() ^ get_down_face();
}

pga3::Bivec BoxInterface::get_outer_left_edge() const
{
    return get_left_face() ^ get_out_face();
}

pga3::Bivec BoxInterface::get_outer_right_edge() const
{
    return get_right_face() ^ get_out_face();
}

pga3::Bivec BoxInterface::get_outer_up_edge() const
{
    return get_up_face() ^ get_out_face();
}

pga3::Bivec BoxInterface::get_outer_down_edge() const
{
    return get_down_face() ^ get_out_face();
}

pga3::Bivec BoxInterface::get_inner_left_edge() const
{
    return get_left_face() ^ get_in_face();
}

pga3::Bivec BoxInterface::get_inner_right_edge() const
{
    return get_right_face() ^ get_in_face();
}

pga3::Bivec BoxInterface::get_inner_up_edge() const
{
    return get_up_face() ^ get_in_face();
}

pga3::Bivec BoxInterface::get_inner_down_edge() const
{
    return get_down_face() ^ get_in_face();
}

pga3::Trivec BoxInterface::get_outer_upper_left_base() const
{
    return get_left_face() ^ get_up_face() ^ get_out_face();
}

pga3::Trivec BoxInterface::get_outer_upper_right_base() const
{
    return get_box().second;
}

pga3::Trivec BoxInterface::get_outer_lower_left_base() const
{
    return get_left_face() ^ get_down_face() ^ get_out_face();
}

pga3::Trivec BoxInterface::get_outer_lower_right_base() const
{
    return get_right_face() ^ get_down_face() ^ get_out_face();
}

pga3::Trivec BoxInterface::get_inner_upper_left_base() const
{
    return get_left_face() ^ get_up_face() ^ get_in_face();
}

pga3::Trivec BoxInterface::get_inner_upper_right_base() const
{
    return get_right_face() ^ get_up_face() ^ get_in_face();
}

pga3::Trivec BoxInterface::get_inner_lower_left_base() const
{
    return get_box().first;
}

pga3::Trivec BoxInterface::get_inner_lower_right_base() const
{
    return get_right_face() ^ get_down_face() ^ get_in_face();
}

pga3::Trivec BoxInterface::get_center_base() const
{
    auto [p1, p2] = get_box();
    return (p1 + p2) / 2;
}

pga3::Trivec BoxInterface::get_left_base() const
{
    auto face = get_left_face();
    auto center = get_center<PGA3>();
    return (face | center) ^ face;
}

pga3::Trivec BoxInterface::get_right_base() const
{
    auto face = get_right_face();
    auto center = get_center<PGA3>();
    return (face | center) ^ face;
}

pga3::Trivec BoxInterface::get_up_base() const
{
    auto face = get_up_face();
    auto center = get_center<PGA3>();
    return (face | center) ^ face;
}

pga3::Trivec BoxInterface::get_down_base() const
{
    auto face = get_down_face();
    auto center = get_center<PGA3>();
    return (face | center) ^ face;
}

pga3::Trivec BoxInterface::get_out_base() const
{
    auto face = get_out_face();
    auto center = get_center<PGA3>();
    return (face | center) ^ face;
}

pga3::Trivec BoxInterface::get_in_base() const
{
    auto face = get_in_face();
    auto center = get_center<PGA3>();
    return (face | center) ^ face;
}

pga2::Vec BoxInterface::get_left_edge() const
{
    return pga3_to_pga2(get_left_face());
}

pga2::Vec BoxInterface::get_right_edge() const
{
    return pga3_to_pga2(get_right_face());
}

pga2::Vec BoxInterface::get_up_edge() const
{
    return pga3_to_pga2(get_up_face());
}

pga2::Vec BoxInterface::get_down_edge() const
{
    return pga3_to_pga2(get_down_face());
}

double BoxInterface::get_x() const
{
    return get_center<PGA3>().undual().blade_project<e1>();
}

double BoxInterface::get_y() const
{
    return get_center<PGA3>().undual().blade_project<e2>();
}

double BoxInterface::get_z() const
{
    return get_center<PGA3>().undual().blade_project<e3>();
}

pga3::Trivec BoxInterface::get_upper_left_base() const
{
    return (get_outer_upper_left_base() + get_inner_upper_left_base()) / 2;
}

pga3::Trivec BoxInterface::get_upper_right_base() const
{
    return (get_outer_upper_right_base() + get_inner_upper_right_base()) / 2;
}

pga3::Trivec BoxInterface::get_lower_left_base() const
{
    return (get_outer_lower_left_base() + get_inner_lower_left_base()) / 2;
}

pga3::Trivec BoxInterface::get_lower_right_base() const
{
    return (get_outer_lower_right_base() + get_inner_lower_right_base()) / 2;
}

pga3::Trivec BoxInterface::get_outside_point_base(const pga3::Trivec& point) const
{
    using namespace pga3;
    auto p = point;
    p -= p.blade_project<e123>() * e123;
    auto center = get_center<PGA3>();
    auto line = center & p;
    auto result = Trivec();
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

double BoxInterface::get_width() const
{
    auto [p1, p2] = get_box();
    return (p2 - p1).undual().blade_project<e1>();
}

double BoxInterface::get_height() const
{
    auto [p1, p2] = get_box();
    return (p2 - p1).undual().blade_project<e2>();
}

double BoxInterface::get_thickness() const
{
    auto [p1, p2] = get_box();
    return (p2 - p1).undual().blade_project<e3>();
}

Box ganim::merge_boxes(const Box& box1, const Box& box2)
{
    auto p11 = box1.get_inner_lower_left().undual();
    auto p12 = box1.get_outer_upper_right().undual();
    auto p21 = box2.get_inner_lower_left().undual();
    auto p22 = box2.get_outer_upper_right().undual();
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
    auto convert_point = [&](vga3::Vec p) {
        auto new_p = ~rotor * vga3_to_pga3(scale*p) * rotor;
        return pga3_to_vga3(new_p.grade_project<3>());
    };
    auto points = std::array<vga3::Vec, 8>();
    int i = 0;
    auto p1 = box.get_inner_lower_left().undual();
    auto p2 = box.get_outer_upper_right().undual();
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
        | std::views::transform(&vga3::Vec::blade_project<vga3::e1>);
    auto ys = transformed_points
        | std::views::transform(&vga3::Vec::blade_project<vga3::e2>);
    auto zs = transformed_points
        | std::views::transform(&vga3::Vec::blade_project<vga3::e3>);
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

Box::Box() : p1(e123), p2(e123) {}

Box::Box(const pga3::Trivec& p1, const pga3::Trivec& p2)
    : p1(p1), p2(p2)
{
    this->p1 /= p1.blade_project<e123>();
    this->p2 /= p2.blade_project<e123>();
}

std::pair<pga3::Trivec, pga3::Trivec> Box::get_box() const
{
    return {p1, p2};
}
