#include "text_base.hpp"

using namespace ganim;

void TextBase::create(const std::vector<std::string_view>& strings)
{
    auto glyphs = get_glyphs(strings);
    const auto size = ssize(strings);
    for (int i = 0; i < size; ++i) {
        auto new_group = ObjectPtr<TextPiece>();
        new_group->draw_together();
        new_group->set_draw_subobject_ratio(0.2);
        add(std::move(new_group));
    }
    auto text_texture = get_text_texture();

    for (auto& glyph : glyphs) {
        auto vertices = std::vector<Shape::Vertex>();
        auto indices = std::vector<unsigned>();
        auto tvertices = std::vector<TextureVertex>();
            vertices.push_back({
                static_cast<float>(glyph.draw_x),
                static_cast<float>(glyph.draw_y),
                0, 0
            });
            vertices.push_back({
                static_cast<float>(glyph.draw_x + glyph.width),
                static_cast<float>(glyph.draw_y),
                0, 1
            });
            vertices.push_back({
                static_cast<float>(glyph.draw_x),
                static_cast<float>(glyph.draw_y - glyph.height),
                0, 2
            });
            vertices.push_back({
                static_cast<float>(glyph.draw_x + glyph.width),
                static_cast<float>(glyph.draw_y - glyph.height),
                0, 3
            });

            tvertices.push_back(
                    {glyph.texture_x, glyph.texture_y});
            tvertices.push_back(
                    {glyph.texture_x + glyph.texture_width,
                     glyph.texture_y});
            tvertices.push_back(
                    {glyph.texture_x,
                     glyph.texture_y + glyph.texture_height});
            tvertices.push_back(
                    {glyph.texture_x + glyph.texture_width,
                     glyph.texture_y + glyph.texture_height});

        if (!glyph.invisible) {
            indices.push_back(0);
            indices.push_back(1);
            indices.push_back(2);
            indices.push_back(2);
            indices.push_back(1);
            indices.push_back(3);
        }

        auto new_glyph = ObjectPtr<TextGlyph>(
            std::move(vertices),
            std::move(indices)
        );
        new_glyph->set_texture_vertices(std::move(tvertices));
        new_glyph->set_texture(text_texture);
        auto true_bounding_box = new_glyph->get_true_bounding_box();
        using namespace vga2;
        auto x_min = pga2_to_vga2(
                true_bounding_box.get_lower_left()).blade_project<e1>();
        auto x_max = pga2_to_vga2(
                true_bounding_box.get_upper_right()).blade_project<e1>();
        auto y_min = glyph.y_min;
        auto y_max = glyph.y_max;
        new_glyph->logical_bounding_box
            = Box(x_min*e1 + y_min*e2, x_max*e1 + y_max*e2);
        (*this)[glyph.group_index]->add(std::move(new_glyph));
    }
    //for (auto& group : *this) {
    //    auto to_remove = std::vector<ObjectPtr<TextGlyph>>();
    //    for (auto& glyph : group) {
    //        if (glyph->invisible) {
    //            to_remove.push_back(glyph);
    //        }
    //    }
    //    group->remove(to_remove);
    //}
}

Box TextGlyph::get_original_logical_bounding_box() const
{
    return logical_bounding_box;
}

ObjectPtr<TextGlyph> TextGlyph::polymorphic_copy() const
{
    return ObjectPtr<TextGlyph>::from_new(polymorphic_copy_impl());
}

TextGlyph* TextGlyph::polymorphic_copy_impl() const
{
    return new TextGlyph(*this);
}

ObjectPtr<TextPiece> TextPiece::polymorphic_copy() const
{
    return ObjectPtr<TextPiece>::from_new(polymorphic_copy_impl());
}

TextPiece* TextPiece::polymorphic_copy_impl() const
{
    return new TextPiece(*this);
}
