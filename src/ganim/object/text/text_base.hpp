#ifndef GANIM_OBJECT_TEXT_BASE_HPP
#define GANIM_OBJECT_TEXT_BASE_HPP

#include "../bases/typed_group.hpp"
#include "../texture_shape.hpp"
#include "text_helpers.hpp"

namespace ganim {
class TextBase : public TypedGroup<TypedGroup<TextureShape<Shape>>> {
    public:
        TextBase()=default;

    protected:
        void create(const std::vector<std::string_view>& strings);

    private:
        virtual std::vector<PositionedGlyph> get_glyphs(
                const std::vector<std::string_view>& strings)=0;

        class TextPiece : public TextureShape<Shape> {
            public:
                using TextureShape<Shape>::TextureShape;
                virtual Box get_original_logical_bounding_box() const override;
                Box logical_bounding_box;

                ObjectPtr<TextPiece> polymorphic_copy() const;
                virtual TextPiece* polymorphic_copy_impl() const;
        };
};
}

#endif
