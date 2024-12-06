#include "conversions.hpp"

namespace ganim {

vga3::Scalar vga2_to_vga3(const vga2::Scalar& in)
{
    return in.coefficient;
}

vga3::Vec vga2_to_vga3(const vga2::Vec& in)
{
    return in.blade_project<vga2::e1>() * vga3::e1 +
           in.blade_project<vga2::e2>() * vga3::e2;
}

vga3::Bivec vga2_to_vga3(const vga2::Bivec& in)
{
    return in.coefficient * vga3::e12;
}

vga3::Even vga2_to_vga3(const vga2::Even& in)
{
    return in.blade_project<vga2::e>() +
           in.blade_project<vga2::e12>() * vga3::e12;
}

vga2::Scalar vga3_to_vga2(const vga3::Scalar& in)
{
    return in.coefficient;
}

vga2::Vec vga3_to_vga2(const vga3::Vec& in)
{
    return in.blade_project<vga3::e1>() * vga2::e1 +
           in.blade_project<vga3::e2>() * vga2::e2;
}

vga2::Bivec vga3_to_vga2(const vga3::Bivec& in)
{
    return in.blade_project<vga3::e12>() * vga2::e12;
}

pga3::Scalar pga2_to_pga3(const pga2::Scalar& in)
{
    return in.coefficient;
}

pga3::Vec pga2_to_pga3(const pga2::Vec& in)
{
    return in.blade_project<pga2::e1>() * pga3::e1 +
           in.blade_project<pga2::e2>() * pga3::e2 +
           in.blade_project<pga2::e0>() * pga3::e0;
}

pga3::Bivec pga2_to_pga3(const pga2::Bivec& in)
{

    return in.blade_project<pga2::e12>() * pga3::e12 +
           in.blade_project<pga2::e01>() * pga3::e01 +
           in.blade_project<pga2::e02>() * pga3::e02;
}

pga3::Even pga2_to_pga3(const pga2::Even& in)
{
    return pga2_to_pga3(in.grade_project<0>()) +
           pga2_to_pga3(in.grade_project<2>());
}

pga3::Bivec pga2_to_pga3_flat(const pga2::Vec& in)
{
    return pga2_to_pga3(in) ^ pga3::e3;
}

pga3::Trivec pga2_to_pga3_flat(const pga2::Bivec& in)
{
    return pga2_to_pga3(in) ^ pga3::e3;
}

pga2::Scalar pga3_to_pga2(const pga3::Scalar& in)
{
    return in.coefficient;
}

pga2::Vec pga3_to_pga2(const pga3::Vec& in)
{
    return in.blade_project<pga3::e1>() * pga2::e1 +
           in.blade_project<pga3::e2>() * pga2::e2 +
           in.blade_project<pga3::e0>() * pga2::e0;
}

pga2::Bivec pga3_to_pga2(const pga3::Bivec& in)
{
    return in.blade_project<pga3::e12>() * pga2::e12 +
           in.blade_project<pga3::e01>() * pga2::e01 +
           in.blade_project<pga3::e02>() * pga2::e02;
}

pga2::Bivec pga3_to_pga2(const pga3::Trivec& in)
{
    return pga3_to_pga2(in.undual()).dual();
}

pga2::Scalar vga2_to_pga2(const vga2::Scalar& in)
{
    return in.coefficient;
}

pga2::Bivec vga2_to_pga2(const vga2::Vec& in)
{
    return (in.blade_project<vga2::e1>() * pga2::e1 +
            in.blade_project<vga2::e2>() * pga2::e2 + pga2::e0).dual();
}

pga2::Even vga2_to_pga2(const vga2::Even& in)
{
    return in.blade_project<vga2::e>() +
           in.blade_project<vga2::e12>() * pga2::e12;
}

pga2::Vec vga2_to_pga2_full(const vga2::Vec& in)
{
    return pga2::e12 & vga2_to_pga2(in);
}

vga2::Scalar pga2_to_vga2(const pga2::Scalar& in)
{
    return in.coefficient;
}

vga2::Vec pga2_to_vga2(const pga2::Bivec& in)
{
    auto p = in.undual();
    auto mag = p.blade_project<pga2::e0>();
    if (mag != 0) p /= mag;
    return p.blade_project<pga2::e1>() * vga2::e1 +
           p.blade_project<pga2::e2>() * vga2::e2;
}

vga2::Vec pga2_to_vga2_cheat(const pga2::Vec& in)
{
    return in.blade_project<pga2::e1>() * vga2::e1 +
           in.blade_project<pga2::e2>() * vga2::e2;
}

pga3::Scalar vga3_to_pga3(const vga3::Scalar& in)
{
    return in.coefficient;
}

pga3::Trivec vga3_to_pga3(const vga3::Vec& in)
{
    return (in.blade_project<vga3::e1>() * pga3::e1 +
            in.blade_project<vga3::e2>() * pga3::e2 +
            in.blade_project<vga3::e3>() * pga3::e3 + pga3::e0).dual();
}

pga3::Vec vga3_to_pga3(const vga3::Bivec& in)
{
    auto v = in.undual();
    return v.blade_project<vga3::e1>() * pga3::e1 +
           v.blade_project<vga3::e2>() * pga3::e2 +
           v.blade_project<vga3::e3>() * pga3::e3;
}

pga3::Even vga3_to_pga3(const vga3::Even& in)
{
    return in.blade_project<vga3::e>() +
           in.blade_project<vga3::e12>() * pga3::e12 +
           in.blade_project<vga3::e13>() * pga3::e13 +
           in.blade_project<vga3::e23>() * pga3::e23;
}

pga3::Bivec vga3_to_pga3_full(const vga3::Vec& in)
{
    return pga3::e123 & vga3_to_pga3(in);
}

vga3::Scalar pga3_to_vga3(const pga3::Scalar& in)
{
    return in.coefficient;
}

vga3::Vec pga3_to_vga3(const pga3::Trivec& in)
{
    auto p = in.undual();
    auto mag = p.blade_project<pga3::e0>();
    if (mag != 0) p /= mag;
    return p.blade_project<pga3::e1>() * vga3::e1 +
           p.blade_project<pga3::e2>() * vga3::e2 +
           p.blade_project<pga3::e3>() * vga3::e3;
}

vga3::Vec pga3_to_vga3_cheat(const pga3::Vec& in)
{
    return in.blade_project<pga3::e1>() * vga3::e1 +
           in.blade_project<pga3::e2>() * vga3::e2 +
           in.blade_project<pga3::e3>() * vga3::e3;
}

pga3::Scalar vga2_to_pga3(const vga2::Scalar& in)
{
    return in.coefficient;
}

pga3::Trivec vga2_to_pga3(const vga2::Vec& in)
{
    return vga3_to_pga3(vga2_to_vga3(in));
}

pga3::Even vga2_to_pga3(const vga2::Even& in)
{
    return vga3_to_pga3(vga2_to_vga3(in));
}

pga3::Bivec vga2_to_pga3_full(const vga2::Vec& in)
{
    return vga3_to_pga3_full(vga2_to_vga3(in));
}

}
