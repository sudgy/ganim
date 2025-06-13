#ifndef GANIM_GEX_SECTION_HPP
#define GANIM_GEX_SECTION_HPP

#include "ganim/object/text/text_helpers.hpp"

#include "token.hpp"
#include "box.hpp"

namespace ganim::gex {
    struct Section {
        TokenList tokens;
        enum {
            Text,
            InlineMath,
            DisplayMath
        } type;
    };
}

#endif
