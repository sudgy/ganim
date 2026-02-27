#ifndef GANIM_OVERLOADED_HPP
#define GANIM_OVERLOADED_HPP

namespace ganim {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}

#endif
