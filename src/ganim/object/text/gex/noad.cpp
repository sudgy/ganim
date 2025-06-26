#include "noad.hpp"

using namespace ganim;
using namespace ganim::gex;

std::pair<double, bool> gex::get_atom_spacing(AtomType left, AtomType right)
{
    using enum AtomType;
    if (int(left) > int(Inner)) left = Ord;
    if (int(right) > int(Inner)) right = Ord;
    switch (left) {
        case Ord:
            switch (right) {
                case Ord:
                    return {0.5, true};
                case Op:
                    return {4.5, true};
                case Bin:
                    return {5.5, false};
                case Rel:
                    return {6.5, false};
                case Open:
                    return {1.5, false};
                case Close:
                    return {1.5, false};
                case Punct:
                    return {1.5, false};
                case Inner:
                    return {4.5, false};
                default:
                    return {1.5, false};
            }
        case Op:
            switch (right) {
                case Ord:
                    return {4.5, true};
                case Op:
                    return {4.5, true};
                case Bin:
                    return {1.5, false};
                case Rel:
                    return {6.5, false};
                case Open:
                    return {1.5, false};
                case Close:
                    return {1.5, false};
                case Punct:
                    return {1.5, false};
                case Inner:
                    return {4.5, false};
                default:
                    return {1.5, false};
            }
        case Bin:
            switch (right) {
                case Ord:
                    return {5.5, false};
                case Op:
                    return {5.5, false};
                case Bin:
                    return {1.5, false};
                case Rel:
                    return {1.5, false};
                case Open:
                    return {5.5, false};
                case Close:
                    return {1.5, false};
                case Punct:
                    return {1.5, false};
                case Inner:
                    return {5.5, false};
                default:
                    return {1.5, false};
            }
        case Rel:
            switch (right) {
                case Ord:
                    return {6.5, false};
                case Op:
                    return {6.5, false};
                case Bin:
                    return {1.5, false};
                case Rel:
                    return {1.5, false};
                case Open:
                    return {6.5, false};
                case Close:
                    return {1.5, false};
                case Punct:
                    return {1.5, false};
                case Inner:
                    return {6.5, false};
                default:
                    return {1.5, false};
            }
        case Open:
            switch (right) {
                case Ord:
                    return {1.5, false};
                case Op:
                    return {1.5, false};
                case Bin:
                    return {1.5, false};
                case Rel:
                    return {1.5, false};
                case Open:
                    return {1.5, false};
                case Close:
                    return {1.5, false};
                case Punct:
                    return {1.5, false};
                case Inner:
                    return {1.5, false};
                default:
                    return {1.5, false};
            }
        case Close:
            switch (right) {
                case Ord:
                    return {1.5, false};
                case Op:
                    return {4.5, true};
                case Bin:
                    return {5.5, false};
                case Rel:
                    return {6.5, false};
                case Open:
                    return {1.5, false};
                case Close:
                    return {1.5, false};
                case Punct:
                    return {1.5, false};
                case Inner:
                    return {4.5, false};
                default:
                    return {1.5, false};
            }
        case Punct:
            switch (right) {
                case Ord:
                    return {4.5, false};
                case Op:
                    return {4.5, false};
                case Bin:
                    return {1.5, false};
                case Rel:
                    return {4.5, false};
                case Open:
                    return {4.5, false};
                case Close:
                    return {4.5, false};
                case Punct:
                    return {4.5, false};
                case Inner:
                    return {4.5, false};
                default:
                    return {1.5, false};
            }
        case Inner:
            switch (right) {
                case Ord:
                    return {4.5, false};
                case Op:
                    return {4.5, true};
                case Bin:
                    return {5.5, false};
                case Rel:
                    return {6.5, false};
                case Open:
                    return {4.5, false};
                case Close:
                    return {1.5, false};
                case Punct:
                    return {4.5, false};
                case Inner:
                    return {4.5, false};
                default:
                    return {1.5, false};
            }
        default:
            return {1.5, false};
    }
}
