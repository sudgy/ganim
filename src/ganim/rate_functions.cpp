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

double rf::there_and_back(
    double t,
    std::function<double(double)> rate_func
)
{
    t *= 2;
    if (t < 1) return rate_func(t);
    else return 1 - rate_func(t - 1);
}

double rf::rush_into(double t)
{
    return 2*(smoothererstep(t/2+0.5)-0.5);
}
