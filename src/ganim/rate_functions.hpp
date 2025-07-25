#ifndef GANIM_RATE_FUNCTIONS_HPP
#define GANIM_RATE_FUNCTIONS_HPP

/** @file
 * @brief Contains rate functions, used to describe the rate that animations
 * happen
 *
 * A rate function is a function from doubles to doubles, usually such that f(0)
 * = 0 and f(1) = 1.  Note that rate functions are allowed to go outside the
 * range [0, 1].  Particular animations are allowed to assume that these outside
 * values won't happen, in which case you must pass a rate function that doesn't
 * do this to them.
 */

#include <functional>

namespace ganim::rf {
    /** @brief Just the identity function */
    double linear(double t);
    /** @brief The smoothstep function, defined as 3t^2 - 2t^3. */
    double smoothstep(double t);
    /** @brief The smootherstep function, defined as 6t^5 - 15t^4 + 10t^3. */
    double smootherstep(double t);
    /** @brief The smoothererstep function, defined as
     * -20x^7 + 70x^6 - 84x^5 + 35x^4.
     */
    double smoothererstep(double t);
    /** @brief The first half of the cosine, but flipped and scaled. */
    double cosine(double t);
    /** @brief Goes from zero to one and then back to zero.  It will use the
     * passed in function (which defaults to smoothererstep), squished to [0,
     * 0.5] to go there and back.
     */
    double there_and_back(
        double t,
        std::function<double(double)> rate_func = smoothererstep
    );
    double rush_into(double t);
    double rush_out(double t);
}

#endif
