#ifndef GANIM_DRAWABLE_OBJECT_HPP
#define GANIM_DRAWABLE_OBJECT_HPP

#include "object.hpp"
#include "drawable.hpp"

namespace ganim {
    class DrawableObject : public Object, public Drawable {
        public:
            virtual bool is_visible() const override
            {
                return Object::is_visible();
            }
    };
}

#endif
