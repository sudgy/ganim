#include "text_base.hpp"

using namespace ganim;

void TextBase::create(const std::vector<std::string_view>& strings)
{
    auto codepoint_strings = std::vector<std::u32string>();
    codepoint_strings.reserve(strings.size());
    for (auto& str : strings) {
        auto& codepoints = codepoint_strings.emplace_back();
        codepoints.reserve(str.size());
        auto gen = utf8_to_codepoints(str);
        while (auto codepoint = gen()) {
            codepoints.push_back(*codepoint);
        }
    }

    auto glyphs = get_glyphs(codepoint_strings);

    using Vertex = Shape::Vertex;
    auto all_vertices = std::vector<std::vector<Vertex>>();
    auto all_indices = std::vector<std::vector<unsigned>>();
    auto all_tvertices = std::vector<std::vector<TextureVertex>>();
    auto y_mins = std::vector<double>();
    auto y_maxs = std::vector<double>();
    const auto size = ssize(strings);
    all_vertices.resize(size);
    all_indices.resize(size);
    all_tvertices.resize(size);
    y_mins.resize(size, INFINITY);
    y_maxs.resize(size, -INFINITY);
    for (auto& glyph : glyphs) {
        y_mins[glyph.group_index]
            = std::min(y_mins[glyph.group_index], glyph.y_min);
        y_maxs[glyph.group_index]
            = std::max(y_maxs[glyph.group_index], glyph.y_max);

        auto j = all_vertices[glyph.group_index].size();
        all_vertices[glyph.group_index].push_back({
            static_cast<float>(glyph.draw_x),
            static_cast<float>(glyph.draw_y),
            0, static_cast<float>(0 + j*4)
        });
        all_vertices[glyph.group_index].push_back({
            static_cast<float>(glyph.draw_x + glyph.width),
            static_cast<float>(glyph.draw_y),
            0, static_cast<float>(1 + j*4)
        });
        all_vertices[glyph.group_index].push_back({
            static_cast<float>(glyph.draw_x),
            static_cast<float>(glyph.draw_y - glyph.height),
            0, static_cast<float>(2 + j*4)
        });
        all_vertices[glyph.group_index].push_back({
            static_cast<float>(glyph.draw_x + glyph.width),
            static_cast<float>(glyph.draw_y - glyph.height),
            0, static_cast<float>(3 + j*4)
        });

        all_tvertices[glyph.group_index].push_back(
                {glyph.texture_x, glyph.texture_y});
        all_tvertices[glyph.group_index].push_back(
                {glyph.texture_x + glyph.texture_width,
                 glyph.texture_y});
        all_tvertices[glyph.group_index].push_back(
                {glyph.texture_x,
                 glyph.texture_y + glyph.texture_height});
        all_tvertices[glyph.group_index].push_back(
                {glyph.texture_x + glyph.texture_width,
                 glyph.texture_y + glyph.texture_height});

        all_indices[glyph.group_index].push_back(j + 0);
        all_indices[glyph.group_index].push_back(j + 1);
        all_indices[glyph.group_index].push_back(j + 2);
        all_indices[glyph.group_index].push_back(j + 2);
        all_indices[glyph.group_index].push_back(j + 1);
        all_indices[glyph.group_index].push_back(j + 3);
    }
    auto text_texture = get_text_texture();
    M_pieces.reserve(size);
    for (int n = 0; n < size; ++n) {
        auto& new_piece = M_pieces.emplace_back(ObjectPtr<TextPiece>(
            std::move(all_vertices[n]),
            std::move(all_indices[n])
        ));
        add(new_piece);
        new_piece->set_texture_vertices(std::move(all_tvertices[n]));
        new_piece->set_texture(text_texture);
        auto true_bounding_box = new_piece->get_true_bounding_box();
        using namespace vga2;
        auto x_min = pga2_to_vga2(
                true_bounding_box.get_lower_left()).blade_project<e1>();
        auto x_max = pga2_to_vga2(
                true_bounding_box.get_upper_right()).blade_project<e1>();
        auto y_min = y_mins[n];
        auto y_max = y_maxs[n];
        new_piece->logical_bounding_box
            = Box(x_min*e1 + y_min*e2, x_max*e1 + y_max*e2);
    }
}

Box TextBase::TextPiece::get_original_logical_bounding_box() const
{
    return logical_bounding_box;
}

ObjectPtr<TextBase::TextPiece> TextBase::TextPiece::polymorphic_copy() const
{
    return ObjectPtr<TextBase::TextPiece>::from_new(polymorphic_copy_impl());
}

TextBase::TextPiece* TextBase::TextPiece::polymorphic_copy_impl() const
{
    return new TextBase::TextPiece(*this);
}
