#include "atom.hpp"

using namespace ganim;
using namespace ganim::gex;

std::pair<int, bool> gex::get_atom_spacing(Atom left, Atom right)
{
    using enum Atom;
    if (int(left) > int(Inner)) left = Ord;
    if (int(right) > int(Inner)) right = Ord;
    switch (left) {
        case Ord:
            switch (right) {
                case Ord:
                    return {0, false};
                case Op:
                    return {1, true};
                case Bin:
                    return {2, false};
                case Rel:
                    return {3, false};
                case Open:
                    return {0, false};
                case Close:
                    return {0, false};
                case Punct:
                    return {0, false};
                case Inner:
                    return {1, false};
                default:
                    return {0, false};
            }
        case Op:
            switch (right) {
                case Ord:
                    return {1, true};
                case Op:
                    return {1, true};
                case Bin:
                    return {0, false};
                case Rel:
                    return {3, false};
                case Open:
                    return {0, false};
                case Close:
                    return {0, false};
                case Punct:
                    return {0, false};
                case Inner:
                    return {1, false};
                default:
                    return {0, false};
            }
        case Bin:
            switch (right) {
                case Ord:
                    return {2, false};
                case Op:
                    return {2, false};
                case Bin:
                    return {0, false};
                case Rel:
                    return {0, false};
                case Open:
                    return {2, false};
                case Close:
                    return {0, false};
                case Punct:
                    return {0, false};
                case Inner:
                    return {2, false};
                default:
                    return {0, false};
            }
        case Rel:
            switch (right) {
                case Ord:
                    return {3, false};
                case Op:
                    return {3, false};
                case Bin:
                    return {0, false};
                case Rel:
                    return {0, false};
                case Open:
                    return {3, false};
                case Close:
                    return {0, false};
                case Punct:
                    return {0, false};
                case Inner:
                    return {3, false};
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
                    return {1, true};
                case Bin:
                    return {2, false};
                case Rel:
                    return {3, false};
                case Open:
                    return {0, false};
                case Close:
                    return {0, false};
                case Punct:
                    return {0, false};
                case Inner:
                    return {1, false};
                default:
                    return {0, false};
            }
        case Punct:
            switch (right) {
                case Ord:
                    return {1, false};
                case Op:
                    return {1, false};
                case Bin:
                    return {0, false};
                case Rel:
                    return {1, false};
                case Open:
                    return {1, false};
                case Close:
                    return {1, false};
                case Punct:
                    return {1, false};
                case Inner:
                    return {1, false};
                default:
                    return {0, false};
            }
        case Inner:
            switch (right) {
                case Ord:
                    return {1, false};
                case Op:
                    return {1, true};
                case Bin:
                    return {2, false};
                case Rel:
                    return {3, false};
                case Open:
                    return {1, false};
                case Close:
                    return {0, false};
                case Punct:
                    return {1, false};
                case Inner:
                    return {1, false};
                default:
                    return {0, false};
            }
        default:
            return {0, false};
    }
}
