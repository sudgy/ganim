#ifndef GANIM_GA_PRINT_H
#define GANIM_GA_PRINT_H

/** @file
 * @brief Contains printing functions for multivectors
 */

#include <ostream>

#include "sga.hpp"

namespace ganim {

/** @brief Outputs a multivector to a stream
 *
 * This function is not meant to be called directly.  Instead, in a particular
 * flavor, you should call this function in an ostream `operator<<`
 * implementation.
 *
 * @tparam names The characters used to represent the different basis vectors.
 * For example, in 3D PGA, the string "0123" is passed.
 * @param stream The stream to output to.
 * @param m The multivector to output.
 */
template <const char* names, scalar Scalar, auto metric, std::uint64_t... bases>
void output_multivector(
    std::ostream& stream,
    const Multivector<Scalar, metric, bases...>& m
)
{
    bool outputted = false;
    [[maybe_unused]] auto process_basis = [&]<std::uint64_t b>() {
        auto coef = m.template binary_blade_project<b>();
        if (coef == 0) return;
        if (outputted) {
            if (coef > 0) stream << " + ";
            else stream << " - ";
        }
        else {
            if (coef < 0) stream << "-";
        }
        if (b == 0 or (coef != 1 and coef != -1)) {
            if (coef > 0) stream << coef;
            else stream << -coef;
        }
        if constexpr (b != 0) {
            stream << "e";
            for (int i = 0; i < ssize(metric); ++i) {
                if ((1 << i) & b) {
                    stream << names[i];
                }
            }
        }
        outputted = true;
    };
    for (int i = 0; i <= ssize(metric); ++i) {
        ((std::popcount(bases) == i ?
          process_basis.template operator()<bases>() : void()), ...);
    }
    if (!outputted) stream << "0";
}

}

#endif
