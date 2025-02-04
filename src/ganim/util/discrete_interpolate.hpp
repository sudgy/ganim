#ifndef GANIM_UTIL_DISCRETE_INTERPOLATE_HPP
#define GANIM_UTIL_DISCRETE_INTERPOLATE_HPP

#include <vector>

namespace ganim {

/** @brief A tool for discretely interpolating two lists
 *
 * The idea behind this function is to take two lists and find a relatively even
 * mapping from the objects in the first list to objects in the second.  You
 * pass in the size of both lists, and this function will return a vector of
 * vectors, where the outer vector has the same size as the first list, and the
 * inner vector will contain indices into the second list that the corresponding
 * element of the first list maps to.
 */
std::vector<std::vector<int>> discrete_interpolate(int from_size, int to_size);

}

#endif
