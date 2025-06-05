#include "bivector.hpp"

#include "vector.hpp"
#include "ganim/math.hpp"

using namespace ganim;

Bivector::Bivector(
    const std::vector<pga2::Bivec>& points,
    BivectorArgs args
)
{
    common_construct(points, pga2_to_vga2(points), args);
}

Bivector::Bivector(
    const std::vector<vga2::Vec>& points,
    BivectorArgs args
)
{
    common_construct(vga2_to_pga2(points), points, args);
}

Bivector::Bivector(
    const std::vector<pga3::Trivec>& points,
    BivectorArgs args
)
{
    using namespace pga3;
    auto b = [&]{
        for (int i = 0; i < ssize(points); ++i) {
            for (int j = i + 1; j < ssize(points); ++j) {
                for (int k = j + 1; k < ssize(points); ++k) {
                    auto p1 = points[i];
                    auto p2 = points[j];
                    auto p3 = points[k];
                    auto plane = p1 & p2 & p3;
                    if (plane.norm2() != 0.0) return plane.normalized();
                }
            }
        }
        throw std::invalid_argument(
            "Unable to find the plane that a 3D bivector is in.  "
            "This could be caused by passing in fewer than three "
            "points or by all of the points being collinear.");
    }();
    auto r = Even(1);
    if ((b + e3).norm2() > 1e-10) {
        r *= -b * (b + e3).normalized();
    }
    auto vga3_flat_points = std::vector<vga3::Vec>();
    vga3_flat_points.resize(points.size());
    for (int i = 0; i < ssize(points); ++i) {
        vga3_flat_points[i]
            = pga3_to_vga3((~r * points[i] * r).grade_project<3>());
    }
    auto vga2_points = vga3_to_vga2(vga3_flat_points);
    auto pga2_points = vga2_to_pga2(vga2_points);
    common_construct(pga2_points, vga2_points, args);
    apply_rotor(~r);
}

Bivector::Bivector(
    const std::vector<vga3::Vec>& points,
    BivectorArgs args
)
{
    using namespace vga3;
    auto b = [&]{
        for (int i = 0; i < ssize(points); ++i) {
            for (int j = i + 1; j < ssize(points); ++j) {
                for (int k = j + 1; k < ssize(points); ++k) {
                    auto p1 = points[i];
                    auto p2 = points[j];
                    auto p3 = points[k];
                    auto plane = (p2 - p1) ^ (p3 - p2);
                    if (plane.norm2() != 0.0) return plane.normalized();
                }
            }
        }
        throw std::invalid_argument(
            "Unable to find the plane that a 3D bivector is in.  "
            "This could be caused by passing in fewer than three "
            "points or by all of the points being collinear.");
    }();
    auto r = Even(1);
    if ((b + e12).norm2() > 1e-10) {
        r *= -b * (b + e12).normalized();
    }
    auto vga3_flat_points = std::vector<vga3::Vec>();
    vga3_flat_points.resize(points.size());
    for (int i = 0; i < ssize(points); ++i) {
        vga3_flat_points[i]
            = (~r * points[i] * r).grade_project<1>();
    }
    auto vga2_points = vga3_to_vga2(vga3_flat_points);
    auto pga2_points = vga2_to_pga2(vga2_points);
    common_construct(pga2_points, vga2_points, args);
    apply_rotor(~r);
}

Bivector::Bivector(
    const Vector& v1,
    const Vector& v2,
    BivectorArgs args
) : Bivector(
        v1.get_end_vga3() - v1.get_start_vga3(),
        v2.get_end_vga3() - v2.get_start_vga3(),
        args
    )
{
    shift(v1.get_start_vga3());
}

Bivector::Bivector(
    vga2::Vec v1,
    vga2::Vec v2,
    BivectorArgs args
)
:   Bivector(vga2_to_vga3(v1), vga2_to_vga3(v2), args) {}

Bivector::Bivector(
    vga3::Vec v1,
    vga3::Vec v2,
    BivectorArgs args
)
{
    using namespace vga3;
    const auto b = (v1 ^ v2).normalized();
    auto r = Even(1);
    if ((b + e12).norm2() > 1e-10) {
        r *= -b * (b + e12).normalized();
    }
    auto vga3_flat_points = std::vector{
        0*e1,
        (~r*v1*r).grade_project<1>(),
        (~r*(v1 + v2)*r).grade_project<1>(),
        (~r*v2*r).grade_project<1>(),
    };
    auto vga2_points = vga3_to_vga2(vga3_flat_points);
    auto pga2_points = vga2_to_pga2(vga2_points);
    common_construct(pga2_points, vga2_points, args);
    apply_rotor(~r);
}

Bivector::Bivector(vga2::Bivec b, BivectorArgs args)
: Bivector(
    b.blade_project<vga2::e12>() >= 0 ?
        std::sqrt(b.norm())*vga3::e1 : std::sqrt(b.norm())*vga3::e2,
    b.blade_project<vga2::e12>() >= 0 ?
        std::sqrt(b.norm())*vga3::e2 : std::sqrt(b.norm())*vga3::e1,
    args
)
{
    if (b.blade_project<vga2::e12>() < 0) {

    }
}

Bivector::Bivector(vga3::Bivec b, BivectorArgs args)
: Bivector(
    std::sqrt(b.norm())*vga3::e1,
    std::sqrt(b.norm())*vga3::e2,
    args
)
{
    using namespace vga3;
    auto mid = e12 + b.normalized();
    if (mid.norm2() < 1e-10) {
        rotate(τ/2, (e1 + e2).normalized().dual());
    }
    else {
        apply_rotor(e21 * (e12 + b.normalized()).normalized());
    }
}

void Bivector::common_construct(
    const std::vector<pga2::Bivec>& pga_points,
    const std::vector<vga2::Vec>& vga_points,
    BivectorArgs args
)
{
    if (ssize(pga_points) < 3) {
        throw std::invalid_argument(
                "Bivectors must have at least three vertices.");
    }
    auto inside = make_polygon_shape(pga_points);
    inside->set_color("FFFFFF7F");
    auto outside = make_group();
    auto path_args = ArrowPathArgs{
        .tip_size = args.tip_size,
        .thickness = args.thickness
    };
    auto minimum_length = args.tip_size / args.max_tip_to_length_ratio;

    double current_path_length = 0.0;
    int current_path_start = 0;
    auto new_path = std::vector<vga2::Vec>();
    auto outside_paths = std::vector<ObjectPtr<ArrowPath>>();
    for (int i = 0; i < ssize(vga_points); ++i) {
        auto i2 = (i + 1) % ssize(vga_points);
        auto i3 = (i + 2) % ssize(vga_points);
        auto this_length = (vga_points[i] - vga_points[i2]).norm();
        current_path_length += this_length;
        if (i2 == 0) break;
        if (current_path_length > minimum_length) {
            auto v1 = (vga_points[i2] - vga_points[i]).normalized();
            auto v2 = (vga_points[i3] - vga_points[i2]).normalized();
            // The path is making a noticeable turn
            if ((v1 | v2).coefficient < 0.9) {
                new_path.clear();
                new_path.reserve(i2 - current_path_start + 1);
                for (int j = current_path_start; j <= i2; ++j) {
                    new_path.push_back(vga_points[j]);
                }
                outside_paths.emplace_back(make_arrow_path(
                            new_path, path_args));
                current_path_length = 0.0;
                current_path_start = i2;
            }
        }
    }
    new_path.clear();
    new_path.reserve(ssize(vga_points) - current_path_start + 1);
    for (int j = current_path_start; j < ssize(vga_points); ++j) {
        new_path.push_back(vga_points[j]);
    }
    new_path.push_back(vga_points[0]);
    // Maybe find a way to deal with this not being long enough?
    outside_paths.emplace_back(make_arrow_path(
                new_path, path_args));
    for (auto& path : outside_paths) {
        outside->add(path);
    }
    set(inside, outside);
}

Bivector& Bivector::set_color(Color color)
{
    Object::set_color(color);
    if (propagate()) {
        get_outside()->set_color(color);
        color.a /= 2;
        get_inside()->set_color(color);
    }
    return *this;
}

ObjectPtr<Bivector> Bivector::polymorphic_copy() const
{
    return ObjectPtr<Bivector>::from_new(polymorphic_copy_impl());
}

Bivector* Bivector::polymorphic_copy_impl() const
{
    auto result = std::make_unique<Bivector>(*this);
    result->set(
        get_inside()->polymorphic_copy(),
        get_outside()->polymorphic_copy()
    );
    return result.release();
}
