#include "discrete_interpolate.hpp"

#include <stdexcept>

std::vector<std::vector<int>>
ganim::discrete_interpolate(int from_size, int to_size)
{
    if (from_size == 0) throw std::invalid_argument(
            "A from size of 0 was passed to discrete_interpolate");
    if (to_size == 0) throw std::invalid_argument(
            "A from size of 0 was passed to discrete_interpolate");
    auto result = std::vector<std::vector<int>>();
    result.resize(from_size);
    auto frac = double(to_size) / double(from_size);
    for (int i = 0; i < from_size; ++i) {
        auto this_i = int(i * frac);
        auto next_i = int((i+1) * frac);
        result[i].push_back(this_i);
        for (int j = this_i + 1; j < next_i; ++j) result[i].push_back(j);
    }
    return result;
}
