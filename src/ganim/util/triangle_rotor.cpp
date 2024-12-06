#include "triangle_rotor.hpp"

#include "ganim/math.hpp"
#include "ganim/ga/exp.hpp"

using namespace ganim;
using namespace pga3;

Even ganim::triangle_rotor(
    Trivec a1,
    Trivec a2,
    Trivec a3,
    Trivec b1,
    Trivec b2,
    Trivec b3
)
{
    a1 /= a1.blade_project<e123>();
    a2 /= a2.blade_project<e123>();
    a3 /= a3.blade_project<e123>();
    b1 /= b1.blade_project<e123>();
    b2 /= b2.blade_project<e123>();
    b3 /= b3.blade_project<e123>();

    auto r1 = Even(-a1 * (a1 + b1) / 2);
    a1 = b1;
    a2 = (~r1 * a2 * r1).grade_project<3>();
    a3 = (~r1 * a3 * r1).grade_project<3>();

    auto r2 = Even();
    auto a12 = (a1 & a2).normalized();
    auto b12 = (b1 & b2).normalized();
    auto ab12 = a12 + b12;
    if (ab12.norm2() < 1e-10) {
        auto p = a12 | a1;
        auto axis = (((a1 & a3) | p) ^ p).normalized();
        r2 = ga_exp(axis*τ/4);
    }
    else {
        r2 = Even(-a12 * (a12 + b12).normalized());
    }
    a2 = b2;
    a3 = (~r2 * a3 * r2).grade_project<3>();

    auto r3 = Even();
    auto a123 = (a1 & a2 & a3).normalized();
    auto b123 = (b1 & b2 & b3).normalized();
    auto ab123 = a123 + b123;
    if (ab123.norm2() < 1e-10) {
        r3 = ga_exp(b12*τ/4);
    }
    else {
        r3 = Even(a123 * (a123 + b123).normalized());
    }

    return r1 * r2 * r3;
}
