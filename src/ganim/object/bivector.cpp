#include "bivector.hpp"

#include "vector.hpp"

using namespace ganim;

BivectorObject::BivectorObject(
    const std::vector<pga2::Bivector>& points,
    BivectorArgs args
)
{
    common_construct(points, pga2_to_vga2(points), args);
}

BivectorObject::BivectorObject(
    const std::vector<vga2::Vector>& points,
    BivectorArgs args
)
{
    common_construct(vga2_to_pga2(points), points, args);
}

BivectorObject::BivectorObject(
    const VectorObject& v1,
    const VectorObject& v2,
    BivectorArgs args
) : BivectorObject(
        v1.get_end_vga3() - v1.get_start_vga3(),
        v2.get_end_vga3() - v2.get_start_vga3(),
        args
    )
{
    shift(v1.get_start_vga3());
}

BivectorObject::BivectorObject(
    vga2::Vector v1,
    vga2::Vector v2,
    BivectorArgs args
)
:   BivectorObject(vga2_to_vga3(v1), vga2_to_vga3(v2), args) {}

BivectorObject::BivectorObject(
    vga3::Vector v1,
    vga3::Vector v2,
    BivectorArgs args
)
{
    using namespace vga3;
    const auto b = (v1 ^ v2).normalized();
    const auto r = -b * (b + e12).normalized();
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

BivectorObject::BivectorObject(vga2::Bivector b, BivectorArgs args)
: BivectorObject(
    std::sqrt(b.norm())*vga3::e1,
    std::sqrt(b.norm())*vga3::e2,
    args
) {}

BivectorObject::BivectorObject(vga3::Bivector b, BivectorArgs args)
: BivectorObject(
    std::sqrt(b.norm())*vga3::e1,
    std::sqrt(b.norm())*vga3::e2,
    args
)
{
    using namespace vga3;
    apply_rotor(e21 * (e12 + b.normalized()).normalized());
}

void BivectorObject::common_construct(
    const std::vector<pga2::Bivector>& pga_points,
    const std::vector<vga2::Vector>& vga_points,
    BivectorArgs args
)
{
    if (ssize(pga_points) < 3) {
        throw std::invalid_argument(
                "Bivectors must have at least three vertices.");
    }
    M_inside = std::make_unique<PolygonShape>(pga_points);
    M_inside->set_opacity(0.5);
    M_outside = std::make_unique<Group>();
    auto path_args = ArrowPathArgs{
        .tip_size = args.tip_size,
        .thickness = args.thickness
    };
    auto minimum_length = args.tip_size / args.max_tip_to_length_ratio;

    double current_path_length = 0.0;
    int current_path_start = 0;
    auto new_path = std::vector<vga2::Vector>();
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
                M_outside_paths.emplace_back(new_path, path_args);
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
    M_outside_paths.emplace_back(new_path, path_args);
    for (auto& path : M_outside_paths) {
        M_outside->add(path);
    }
    add(*M_inside);
    add(*M_outside);
}
