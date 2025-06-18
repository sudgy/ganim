#ifndef GANIM_GEX_NOAD_HPP
#define GANIM_GEX_NOAD_HPP

#include <vector>
#include <variant>
#include <cstdint>

#include "box.hpp"

namespace ganim::gex {
    enum class AtomType {
        Ord,   // Ordinary symbol like x
        Op,    // Large operation like ∑
        Bin,   // Binary operation like +
        Rel,   // Relation like =
        Open,  // Something like (
        Close, // Something like )
        Punct, // Something like ,
        Inner, // Fractions?
        Over,  // Overline
        Under, // Underline
        Acc,   // Accent
        Rad,   // Radical
        Vcent  // Produced by \vcenter
    };
    // Boolean says whether to add the spacing for script styles
    std::pair<int, bool> get_atom_spacing(AtomType left, AtomType right);


    class Noad;
    using MathList = std::vector<Noad>;

    struct AtomFieldSymbol {
        std::uint32_t codepoint = 0;
    };
    struct AtomFieldBox {
        // Box is already in AtomField
    };
    struct AtomFieldList {
        MathList list;
    };
    struct AtomField {
        std::variant<AtomFieldSymbol, AtomFieldBox, AtomFieldList> value;
        Box box;
    };

    struct Atom {
        Box box;
        AtomType type;
        AtomField nucleus;
        // Coming soon: Subscript and superscript
    };

    struct Noad {
        std::variant<Atom> value;
        int group = -1;
        int string_index = -1;
    };
}

#endif
