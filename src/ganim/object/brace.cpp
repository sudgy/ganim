#include "brace.hpp"

#include "ganim/ga/exp.hpp"

using namespace ganim;

namespace {
    std::vector<pga2::Bivec> get_points(
        pga2::Bivec p1,
        pga2::Bivec p2,
        const BraceArgs& args,
        bool squished = false
    )
    {
        using namespace pga2;
        auto distance = (p2 - p1).undual().norm();
        auto trans_b = ga_log(-p1*p2) / distance;
        auto p12 = (p1 & p2).normalized();
        if (!squished and distance <= args.width * 3) {
            const auto squish = distance / (args.width * 3);
            p2 -= p1;
            p2 /= squish;
            p2 += p1;
            auto new_points = get_points(p1, p2, args, true);
            for (auto& point : new_points) {
                auto p_point = (point | p12) ^ p12;
                p_point /= p_point.blade_project<e12>();
                p_point -= p1;
                p_point *= squish;
                p_point += p1;
                auto p_line = p_point | p12;
                point = (point | p_line) ^ p_line;
            }
            return new_points;
        }

        const auto precision = args.precision;
        auto result = std::vector<Bivec>();
        auto sign = args.left ? 1 : -1;

        auto trans_rotor = ga_exp(trans_b * args.width / 2 / 2);
        auto circ_center = (~trans_rotor * p1 * trans_rotor).grade_project<2>();
        for (int i = 0; i <= precision; ++i) {
            if (i == precision and squished) break;
            auto rotor = ga_exp(-sign * circ_center * τ * i / precision /4 /2);
            result.push_back((~rotor * p1 * rotor).grade_project<2>());
        }

        auto outer_edge = p12 - args.width*e0*sign;
        auto bisector = ((p1 + p2) | outer_edge).normalized();
        auto circ_center2 = (bisector + args.width*e0) ^ outer_edge;
        auto midline = (p12 + outer_edge) / 2;
        auto p3 = (circ_center2 | midline) ^ midline;
        for (int i = 0; i <= precision; ++i) {
            auto rotor1 = ga_exp(sign * circ_center2 * τ * i / precision /4 /2);
            auto rotor2 = ga_exp(trans_b * args.width * i / precision / 2 / 2);
            auto rotor = rotor1 * rotor2;
            result.push_back((~rotor * p3 * rotor).grade_project<2>());
        }

        auto size = ssize(result);
        for (int i = 0; i < size - 1; ++i) {
            auto p = result[size-2 - i];
            result.push_back(-(bisector*p*bisector).grade_project<2>());
        }
        return result;
    }
}

Brace::Brace(
    vga2::Vec p1,
    vga2::Vec p2,
    BraceArgs args
)
: Path(get_points(to_pga2(p1), to_pga2(p2), args), false, args.thickness) {}

void Brace::recreate(vga2::Vec p1, vga2::Vec p2, BraceArgs args)
{
    Path::recreate(
            get_points(to_pga2(p1), to_pga2(p2), args), false, args.thickness);
}
