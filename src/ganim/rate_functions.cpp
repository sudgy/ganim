#include "rate_functions.hpp"

using namespace ganim;

double rf::linear(double t)
{
    return t;
}

double rf::smoothstep(double t)
{
    return t*t*(3 - 2*t);
}
