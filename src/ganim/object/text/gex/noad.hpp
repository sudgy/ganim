#ifndef GANIM_GEX_NOAD_HPP
#define GANIM_GEX_NOAD_HPP

#include <vector>
#include <variant>
#include <cstdint>
#include <memory>

#include "box.hpp"
#include "ganim/copy_ptr.hpp"

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
    std::pair<double, bool> get_atom_spacing(AtomType left, AtomType right);


    class Noad;
    class Atom;
    using MathList = std::vector<Noad>;

    struct AtomEmpty {};
    struct AtomSymbol {
        std::uint32_t codepoint = 0;
        int group = -1;
        int string_index = -1;
    };
    struct AtomBox {
        // Box is already in AtomField
    };
    struct AtomList {
        MathList list;
    };
    struct AtomSubscript {
        copy_ptr<Atom> nucleus;
        copy_ptr<Atom> subscript;
    };
    struct AtomSuperscript {
        copy_ptr<Atom> nucleus;
        copy_ptr<Atom> superscript;
    };
    struct AtomSubsuperscript {
        copy_ptr<Atom> nucleus;
        copy_ptr<Atom> subscript;
        copy_ptr<Atom> superscript;
    };

    struct Atom {
        Box box;
        AtomType type;
        std::variant<
            AtomEmpty,
            AtomSymbol,
            AtomBox,
            AtomList,
            AtomSubscript,
            AtomSuperscript,
            AtomSubsuperscript
        > value;
    };
    struct CommandNoad {
        std::string command;
        int group = -1;
        int string_index = -1;
    };

    struct Noad {
        std::variant<Atom, CommandNoad> value;
    };
}

#endif
