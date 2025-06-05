#include "arrow_path.hpp"

#include "ganim/ga/exp.hpp"

using namespace ganim;

ArrowPath::ArrowPath(
    const std::vector<pga2::Bivec>& points,
    ArrowPathArgs args
)
{
    recreate(points, args);
}

ArrowPath::ArrowPath(
    const std::vector<vga2::Vec>& points,
    ArrowPathArgs args
)
{
    recreate(points, args);
}

void ArrowPath::recreate(
    std::vector<pga2::Bivec> points,
    ArrowPathArgs args
)
{
    using namespace pga2;
    auto end_point = points.back();
    end_point /= end_point.blade_project<e12>();
    const auto size2 = args.tip_size * args.tip_size;
    auto final_index = -1;
    for (int i = ssize(points) - 2; i >= 0; --i) {
        if ((points[i] - end_point).undual().norm2() > size2) {
            final_index = i;
            break;
        }
    }
    if (final_index == -1) {
        throw std::invalid_argument("Unable to find a position for the tip of "
                "an ArrowPath.  Is the path too small?");
    }
    points.resize(final_index + 1);
    auto final_line = (end_point & points.back()).normalized();
    auto tip_point = end_point + (final_line ^ e0 * args.tip_size);
    points.push_back(tip_point);
    auto path = make_path(points, false, args.thickness);

    auto r = Even(1);
    r *= e21 * (e12 + tip_point) / 2;
    const auto bad_line = (e1 | tip_point).normalized();
    const auto good_line = (tip_point & end_point).normalized();
    if ((bad_line + good_line).norm() < 1e-5) {
        r *= ga_exp(tip_point*τ/4);
    }
    else {
        r *= bad_line * (bad_line + good_line).normalized();
    }

    auto transform = [&](pga2::Vec p){
        return (~r*p.dual()*r).grade_project<2>().undual();
    };
    const auto p1 = transform( args.tip_size/2*e2 + e0);
    const auto p2 = transform(-args.tip_size/2*e2 + e0);
    const auto p3 = transform( args.tip_size  *e1 + e0);
    const auto x1 = float(p1.blade_project<e1>());
    const auto y1 = float(p1.blade_project<e2>());
    const auto x2 = float(p2.blade_project<e1>());
    const auto y2 = float(p2.blade_project<e2>());
    const auto x3 = float(p3.blade_project<e1>());
    const auto y3 = float(p3.blade_project<e2>());

    auto tip = make_shape(
        {
            {x1, y1, 0, 0},
            {x2, y2, 0, 0},
            {x3, y3, 0, 1}
        },
        {0, 1, 2}
    );

    set(path, tip);
}

void ArrowPath::recreate(
    const std::vector<vga2::Vec>& points,
    ArrowPathArgs args
)
{
    auto pga_points = std::vector<pga2::Bivec>();
    pga_points.reserve(points.size());
    for (auto p : points) {
        pga_points.push_back(vga2_to_pga2(p));
    }
    recreate(pga_points, args);
}

ObjectPtr<ArrowPath> ArrowPath::polymorphic_copy() const
{
    return ObjectPtr<ArrowPath>::from_new(polymorphic_copy_impl());
}

ArrowPath* ArrowPath::polymorphic_copy_impl() const
{
    auto result = std::make_unique<ArrowPath>(*this);
    result->set(
        get<0>()->polymorphic_copy(),
        get<1>()->polymorphic_copy()
    );
    return result.release();
}
