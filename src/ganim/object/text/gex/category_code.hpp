#ifndef GANIM_GEX_CATEGORY_CODE_HPP
#define GANIM_GEX_CATEGORY_CODE_HPP

namespace ganim::gex {
    enum class CategoryCode {
        Escape = 0,
        StartGroup = 1,
        EndGroup = 2,
        MathShift = 3,
        Alignment = 4,
        EndLine = 5,
        MacroParameter = 6,
        Superscript = 7,
        Subscript = 8,
        Ignored = 9,
        Spacer = 10,
        Letter = 11,
        Other = 12,
        Active = 13,
        Comment = 14,
        Invalid = 15
    };
}

#endif
