#ifndef GANIM_OBJECT_TEXT_HPP
#define GANIM_OBJECT_TEXT_HPP

/** @file
 * @brief Contains the @ref ganim::Text class
 */

#include "../bases/group.hpp"
#include "../texture_shape.hpp"

namespace ganim {
    class Text : public Group {
        public:
            template <typename... Ts>
            explicit Text(Ts&&... strings)
                requires(std::convertible_to<Ts, std::string_view> and ...)
            : Text(static_cast<const std::vector<std::string_view>&>(
                std::vector<std::string_view>{std::forward<Ts>(strings)...})) {}
            explicit Text(const std::vector<std::string_view>& strings);

        private:
            class TextPiece : public TextureShape<Shape> {
                public:
                    using TextureShape<Shape>::TextureShape;
                    virtual Box get_original_logical_bounding_box() const override;
                    Box logical_bounding_box;
            };
            std::vector<ObjectPtr<TextPiece>> M_pieces;
    };

    template <typename... Ts>
    ObjectPtr<Text> make_text(Ts&&... tex_strings)
        requires(std::convertible_to<Ts, std::string_view> and ...)
    {
        return ObjectPtr<Text>(std::forward<Ts>(tex_strings)...);
    }

    inline ObjectPtr<Text> make_text(
        const std::vector<std::string_view>& tex_strings
    )
    {
        return ObjectPtr<Text>(tex_strings);
    }
}

#endif
