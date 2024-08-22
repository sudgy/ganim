#include "box.hpp"

#include <ranges>

#include "ganim/ga/conversions.hpp"

using namespace ganim;
using namespace vga3;

vga3::Vector Box::get_center() const
{
    auto x1 = p1.blade_project<e1>();
    auto y1 = p1.blade_project<e2>();
    auto z1 = p1.blade_project<e3>();
    auto x2 = p2.blade_project<e1>();
    auto y2 = p2.blade_project<e2>();
    auto z2 = p2.blade_project<e3>();
    return {(x1 + x2) / 2, (y1 + y2) / 2, (z1 + z2) / 2};
}

vga3::Vector Box::get_left() const
{
    auto x1 = p1.blade_project<e1>();
    auto y1 = p1.blade_project<e2>();
    auto z1 = p1.blade_project<e3>();
    auto y2 = p2.blade_project<e2>();
    auto z2 = p2.blade_project<e3>();
    return {x1, (y1 + y2) / 2, (z1 + z2) / 2};
}

vga3::Vector Box::get_right() const
{
    auto y1 = p1.blade_project<e2>();
    auto z1 = p1.blade_project<e3>();
    auto x2 = p2.blade_project<e1>();
    auto y2 = p2.blade_project<e2>();
    auto z2 = p2.blade_project<e3>();
    return {x2, (y1 + y2) / 2, (z1 + z2) / 2};
}

vga3::Vector Box::get_up() const
{
    auto x1 = p1.blade_project<e1>();
    auto z1 = p1.blade_project<e3>();
    auto x2 = p2.blade_project<e1>();
    auto y2 = p2.blade_project<e2>();
    auto z2 = p2.blade_project<e3>();
    return {(x1 + x2) / 2, y2, (z1 + z2) / 2};
}

vga3::Vector Box::get_down() const
{
    auto x1 = p1.blade_project<e1>();
    auto y1 = p1.blade_project<e2>();
    auto z1 = p1.blade_project<e3>();
    auto x2 = p2.blade_project<e1>();
    auto z2 = p2.blade_project<e3>();
    return {(x1 + x2) / 2, y1, (z1 + z2) / 2};
}

vga3::Vector Box::get_out() const
{
    auto x1 = p1.blade_project<e1>();
    auto y1 = p1.blade_project<e2>();
    auto x2 = p2.blade_project<e1>();
    auto y2 = p2.blade_project<e2>();
    auto z2 = p2.blade_project<e3>();
    return {(x1 + x2) / 2, (y1 + y2) / 2, z2};
}

vga3::Vector Box::get_in() const
{
    auto x1 = p1.blade_project<e1>();
    auto y1 = p1.blade_project<e2>();
    auto z1 = p1.blade_project<e3>();
    auto x2 = p2.blade_project<e1>();
    auto y2 = p2.blade_project<e2>();
    return {(x1 + x2) / 2, (y1 + y2) / 2, z1};
}

vga3::Vector Box::get_upper_left() const
{
    auto x1 = p1.blade_project<e1>();
    auto z1 = p1.blade_project<e3>();
    auto y2 = p2.blade_project<e2>();
    auto z2 = p2.blade_project<e3>();
    return {x1, y2, (z1 + z2) / 2};
}

vga3::Vector Box::get_upper_right() const
{
    auto z1 = p1.blade_project<e3>();
    auto x2 = p2.blade_project<e1>();
    auto y2 = p2.blade_project<e2>();
    auto z2 = p2.blade_project<e3>();
    return {x2, y2, (z1 + z2) / 2};
}

vga3::Vector Box::get_lower_left() const
{
    auto x1 = p1.blade_project<e1>();
    auto y1 = p1.blade_project<e2>();
    auto z1 = p1.blade_project<e3>();
    auto z2 = p2.blade_project<e3>();
    return {x1, y1, (z1 + z2) / 2};
}

vga3::Vector Box::get_lower_right() const
{
    auto y1 = p1.blade_project<e2>();
    auto z1 = p1.blade_project<e3>();
    auto x2 = p2.blade_project<e1>();
    auto z2 = p2.blade_project<e3>();
    return {x2, y1, (z1 + z2) / 2};
}

Box ganim::merge_boxes(const Box& box1, const Box& box2)
{
    return Box(
        {
            std::min(box1.p1.blade_project<e1>(), box2.p1.blade_project<e1>()),
            std::min(box1.p1.blade_project<e2>(), box2.p1.blade_project<e2>()),
            std::min(box1.p1.blade_project<e3>(), box2.p1.blade_project<e3>())
        },
        {
            std::max(box1.p2.blade_project<e1>(), box2.p2.blade_project<e1>()),
            std::max(box1.p2.blade_project<e2>(), box2.p2.blade_project<e2>()),
            std::max(box1.p2.blade_project<e3>(), box2.p2.blade_project<e3>())
        }
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
    for (auto x : {box.p1.blade_project<e1>(),
                   box.p2.blade_project<e1>()}) {
        for (auto y : {box.p1.blade_project<e2>(),
                       box.p2.blade_project<e2>()}) {
            for (auto z : {box.p1.blade_project<e3>(),
                           box.p2.blade_project<e3>()}) {
                points[i++] = {x, y, z};
            }
        }
    }
    auto transformed_points = points | std::views::transform(convert_point);
    auto xs = transformed_points
        | std::views::transform(&vga3::Vector::blade_project<e1>);
    auto ys = transformed_points
        | std::views::transform(&vga3::Vector::blade_project<e2>);
    auto zs = transformed_points
        | std::views::transform(&vga3::Vector::blade_project<e3>);
    return Box(
        {
            *std::ranges::min_element(xs),
            *std::ranges::min_element(ys),
            *std::ranges::min_element(zs)
        },
        {
            *std::ranges::max_element(xs),
            *std::ranges::max_element(ys),
            *std::ranges::max_element(zs)
        }
    );
}
