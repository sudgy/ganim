#ifndef GANIM_GEX_ATOM_HPP
#define GANIM_GEX_ATOM_HPP

#include <utility>

namespace ganim::gex {
    enum class Atom {
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
    std::pair<int, bool> get_atom_spacing(Atom left, Atom right);
}

#endif
