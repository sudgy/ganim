#include "rate_functions.hpp"

#include <cmath>

#include "math.hpp"

using namespace ganim;

double rf::linear(double t)
{
    return t;
}

double rf::smoothstep(double t)
{
    return t*t*(3 - 2*t);
}

double rf::smootherstep(double t)
{
    return t*t*t*(t*(6*t - 15) + 10);
}

double rf::smoothererstep(double t)
{
    return t*t*t*t*(t*(t*(-20*t + 70) - 84) + 35);
}

double rf::cosine(double t)
{
    return (1 - std::cos(t*τ/2)) / 2;
}
