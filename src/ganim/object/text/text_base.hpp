#ifndef GANIM_OBJECT_TEXT_BASE_HPP
#define GANIM_OBJECT_TEXT_BASE_HPP

#include "../bases/typed_group.hpp"
#include "../texture_shape.hpp"
#include "text_helpers.hpp"

namespace ganim {
class TextGlyph : public TextureShape<Shape> {
    private:
        friend class TextBase;

        using TextureShape<Shape>::TextureShape;
        virtual Box get_original_logical_bounding_box() const override;
        Box logical_bounding_box;

    public:
        ObjectPtr<TextGlyph> copy() const;
        virtual TextGlyph* copy_impl() const;
};
class TextPiece : public TypedGroup<TextGlyph> {
    private:
        friend class TextBase;

    public:
        ObjectPtr<TextPiece> copy() const;
        virtual TextPiece* copy_impl() const;
};
class TextBase : public TypedGroup<TextPiece> {
    public:
        TextBase()=default;

    protected:
        void create(const std::vector<std::string_view>& strings);

    private:
        virtual std::vector<Glyph> get_glyphs(
                const std::vector<std::string_view>& strings)=0;
};
}

#endif
