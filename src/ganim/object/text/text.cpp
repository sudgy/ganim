#include "text.hpp"

#include "text_helpers.hpp"

using namespace ganim;

Text::Text(const std::vector<std::string_view>& strings)
{
    draw_together();
    set_draw_subobject_ratio(0.2);
    using Vertex = Shape::Vertex;
    // TODO: Make this configurable
    auto& font = get_font("fonts/NewCM10-Regular.otf");
    auto shaped_glyphs = shape_text(font, strings);
    auto all_vertices = std::vector<std::vector<Vertex>>();
    auto all_indices = std::vector<std::vector<unsigned>>();
    auto all_tvertices = std::vector<std::vector<TextureVertex>>();
    auto x_min = double(INFINITY);
    auto x_max = double(-INFINITY);
    auto y_mins = std::vector<double>();
    auto y_maxs = std::vector<double>();
    const auto size = ssize(strings);
    all_vertices.resize(size);
    all_indices.resize(size);
    all_tvertices.resize(size);
    y_mins.resize(size, INFINITY);
    y_maxs.resize(size, -INFINITY);
    const auto ascender = get_font_ascender(font);
    const auto descender = get_font_descender(font);
    for (auto& glyph : shaped_glyphs) {
        auto i = glyph.group_index;
        auto& c = *glyph.glyph;
        x_min = std::min(x_min, glyph.x_pos + c.bearing_x);
        x_max = std::max(x_max, glyph.x_pos + c.bearing_x);
        x_min = std::min(x_min, glyph.x_pos + c.bearing_x + c.width);
        x_max = std::max(x_max, glyph.x_pos + c.bearing_x + c.width);
        y_mins[i] = std::min(y_mins[i], glyph.y_pos + descender);
        y_maxs[i] = std::max(y_maxs[i], glyph.y_pos + ascender);
        all_vertices[i].push_back({
            static_cast<float>(glyph.x_pos + c.bearing_x),
            static_cast<float>(c.bearing_y + glyph.y_pos),
            0, static_cast<float>(0 + i*4)
        });
        all_tvertices[i].push_back({c.texture_x, c.texture_y});
        all_vertices[i].push_back({
            static_cast<float>(glyph.x_pos + c.bearing_x + c.width),
            static_cast<float>(c.bearing_y + glyph.y_pos),
            0, static_cast<float>(1 + i*4)
        });
        all_tvertices[i].push_back({c.texture_x + c.texture_width, c.texture_y});
        all_vertices[i].push_back({
            static_cast<float>(glyph.x_pos + c.bearing_x),
            static_cast<float>(c.bearing_y - c.height + glyph.y_pos),
            0, static_cast<float>(2 + i*4)
        });
        all_tvertices[i].push_back({c.texture_x, c.texture_y + c.texture_height});
        all_vertices[i].push_back({
            static_cast<float>(glyph.x_pos + c.bearing_x + c.width),
            static_cast<float>(c.bearing_y - c.height + glyph.y_pos),
            0, static_cast<float>(3 + i*4)
        });
        all_tvertices[i].push_back({c.texture_x + c.texture_width,
                c.texture_y + c.texture_height});
        auto j = all_indices[i].size() / 6 * 4;
        all_indices[i].push_back(j + 0);
        all_indices[i].push_back(j + 1);
        all_indices[i].push_back(j + 2);
        all_indices[i].push_back(j + 2);
        all_indices[i].push_back(j + 1);
        all_indices[i].push_back(j + 3);
    }
    const auto x_shift = -(x_min + x_max) / 2;
    for (auto& vertices : all_vertices) {
        for (auto& vertex : vertices) {
            vertex.x += x_shift;
        }
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

Box Text::TextPiece::get_original_logical_bounding_box() const
{
    return logical_bounding_box;
}
