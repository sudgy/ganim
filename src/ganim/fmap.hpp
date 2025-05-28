#ifndef GANIM_FMAP_HPP
#define GANIM_FMAP_HPP

#include <vector>
#include <list>

template <
    template <typename> typename Container,
    typename In,
    typename Func
>
auto fmap(const Container<In>& input, Func&& func)
{
    using Out = std::remove_cvref_t<std::invoke_result_t<Func, In>>;
    auto result = Container<Out>();
    if constexpr (requires(Container<Out> a) {a.reserve(1);}) {
        result.reserve(input.size());
    }
    for (auto it = input.begin(); it != input.end(); ++it) {
        result.insert(result.end(), func(*it));
    }
    return result;
}

#endif
