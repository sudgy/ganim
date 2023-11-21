#include "approx_color.hpp"

ApproxColor::ApproxColor(ganim::Color color) :
    r(color.r),
    g(color.g),
    b(color.b),
    a(color.a) {}

bool ApproxColor::operator==(const ganim::Color& other) const
{
    auto r_diff = static_cast<int>(r) - static_cast<int>(other.r);
    auto g_diff = static_cast<int>(g) - static_cast<int>(other.g);
    auto b_diff = static_cast<int>(b) - static_cast<int>(other.b);
    auto a_diff = static_cast<int>(a) - static_cast<int>(other.a);
    return std::abs(r_diff) < 16 and
           std::abs(g_diff) < 16 and
           std::abs(b_diff) < 16 and
           std::abs(a_diff) < 16;
}

std::ostream& operator<<(std::ostream& os, const ApproxColor& value)
{
    auto color = ganim::Color(value.r, value.g, value.b, value.a);
    return os << color;
}
