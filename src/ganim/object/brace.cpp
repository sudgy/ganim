#include "brace.hpp"

#include "ganim/ga/exp.hpp"

using namespace ganim;

namespace {
    std::vector<pga2::Bivec> get_points(
        pga2::Bivec p1,
        pga2::Bivec p2,
        const BraceArgs& args
    )
    {
        const auto precision = args.precision;
        using namespace pga2;
        auto result = std::vector<Bivec>();
        auto sign = args.left ? 1 : -1;

        auto distance = (p2 - p1).undual().norm();
        auto trans_b = ga_log(-p1*p2) / distance;
        auto trans_rotor = ga_exp(trans_b * args.width / 2 / 2);
        auto circ_center = (~trans_rotor * p1 * trans_rotor).grade_project<2>();
        for (int i = 0; i <= precision; ++i) {
            auto rotor = ga_exp(-sign * circ_center * τ * i / precision /4 /2);
            result.push_back((~rotor * p1 * rotor).grade_project<2>());
        }

        auto p12 = (p1 & p2).normalized();
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

        for (int i = 0; i < precision*2+1; ++i) {
            auto p = result[precision*2-i];
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
