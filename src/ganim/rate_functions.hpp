#ifndef GANIM_RATE_FUNCTIONS_HPP
#define GANIM_RATE_FUNCTIONS_HPP

/** @file
 * @brief Contains rate functions, used to describe the rate animations happen
 *
 * A rate function is a function from doubles to double such that f(0) = 0 and
 * f(1) = 1.  Note that rate functions are allowed to go outside the range [0,
 * 1].  Particular animations are allowed to assume that these outside values
 * aren't allowed, in which case you must pass a rate function that doesn't do
 * this to them.
 */

namespace ganim::rf {
    /** @brief Just the identity function */
    double linear(double t);
    /** @brief The smoothstep function, defined as 3t^2 - 2t^3. */
    double smoothstep(double t);
}

#endif
