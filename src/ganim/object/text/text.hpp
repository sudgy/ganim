#ifndef GANIM_OBJECT_TEXT_HPP
#define GANIM_OBJECT_TEXT_HPP

/** @file
 * @brief Contains the @ref ganim::Text class
 */

#include "../texture_shape.hpp"

namespace ganim {
    class Text : public TextureShape<Shape> {
        public:
            explicit Text(std::string_view string);
            virtual Box get_original_logical_bounding_box() const override;

        private:
            Box M_logical_bounding_box;
    };
}

#endif
