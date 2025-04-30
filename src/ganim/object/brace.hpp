#ifndef GANIM_OBJECT_BRACE_HPP
#define GANIM_OBJECT_BRACE_HPP

#include "ganim/object/path.hpp"

namespace ganim {
    struct BraceArgs {
        bool left = true;
        double width = 0.3;
        double thickness = 0.04;
        int precision = 32;
    };
    class Brace : public Path {
        public:
            Brace(vga2::Vec p1, vga2::Vec p2, BraceArgs args = {});
    };
    inline ObjectPtr<Brace> make_brace(
        vga2::Vec p1,
        vga2::Vec p2,
        BraceArgs args = {}
    )
    {
        return ObjectPtr<Brace>(p1, p2, args);
    }
}

#endif
