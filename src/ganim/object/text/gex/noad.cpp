#include "noad.hpp"

using namespace ganim;
using namespace ganim::gex;

std::pair<int, bool> gex::get_atom_spacing(AtomType left, AtomType right)
{
    using enum AtomType;
    if (int(left) > int(Inner)) left = Ord;
    if (int(right) > int(Inner)) right = Ord;
    switch (left) {
        case Ord:
            switch (right) {
                case Ord:
                    return {0, false};
                case Op:
                    return {3, true};
                case Bin:
                    return {4, false};
                case Rel:
                    return {5, false};
                case Open:
                    return {0, false};
                case Close:
                    return {0, false};
                case Punct:
                    return {0, false};
                case Inner:
                    return {3, false};
                default:
                    return {0, false};
            }
        case Op:
            switch (right) {
                case Ord:
                    return {3, true};
                case Op:
                    return {3, true};
                case Bin:
                    return {0, false};
                case Rel:
                    return {5, false};
                case Open:
                    return {0, false};
                case Close:
                    return {0, false};
                case Punct:
                    return {0, false};
                case Inner:
                    return {3, false};
                default:
                    return {0, false};
            }
        case Bin:
            switch (right) {
                case Ord:
                    return {4, false};
                case Op:
                    return {4, false};
                case Bin:
                    return {0, false};
                case Rel:
                    return {0, false};
                case Open:
                    return {4, false};
                case Close:
                    return {0, false};
                case Punct:
                    return {0, false};
                case Inner:
                    return {4, false};
                default:
                    return {0, false};
            }
        case Rel:
            switch (right) {
                case Ord:
                    return {5, false};
                case Op:
                    return {5, false};
                case Bin:
                    return {0, false};
                case Rel:
                    return {0, false};
                case Open:
                    return {5, false};
                case Close:
                    return {0, false};
                case Punct:
                    return {0, false};
                case Inner:
                    return {5, false};
                default:
                    return {0, false};
            }
        case Open:
            switch (right) {
                case Ord:
                    return {0, false};
                case Op:
                    return {0, false};
                case Bin:
                    return {0, false};
                case Rel:
                    return {0, false};
                case Open:
                    return {0, false};
                case Close:
                    return {0, false};
                case Punct:
                    return {0, false};
                case Inner:
                    return {0, false};
                default:
                    return {0, false};
            }
        case Close:
            switch (right) {
                case Ord:
                    return {0, false};
                case Op:
                    return {3, true};
                case Bin:
                    return {4, false};
                case Rel:
                    return {5, false};
                case Open:
                    return {0, false};
                case Close:
                    return {0, false};
                case Punct:
                    return {0, false};
                case Inner:
                    return {3, false};
                default:
                    return {0, false};
            }
        case Punct:
            switch (right) {
                case Ord:
                    return {3, false};
                case Op:
                    return {3, false};
                case Bin:
                    return {0, false};
                case Rel:
                    return {3, false};
                case Open:
                    return {3, false};
                case Close:
                    return {3, false};
                case Punct:
                    return {3, false};
                case Inner:
                    return {3, false};
                default:
                    return {0, false};
            }
        case Inner:
            switch (right) {
                case Ord:
                    return {3, false};
                case Op:
                    return {3, true};
                case Bin:
                    return {4, false};
                case Rel:
                    return {5, false};
                case Open:
                    return {3, false};
                case Close:
                    return {0, false};
                case Punct:
                    return {3, false};
                case Inner:
                    return {3, false};
                default:
                    return {0, false};
            }
        default:
            return {0, false};
    }
}
