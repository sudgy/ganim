#include "text.hpp"

#include "text_helpers.hpp"

using namespace ganim;

Text::Text(const std::vector<std::string_view>& strings) : Text(0.0, strings) {}

Text::Text(double newline_buff, const std::vector<std::string_view>& strings)
{
    draw_together();
    set_draw_subobject_ratio(0.2);
    using Vertex = Shape::Vertex;
    // TODO: Make this configurable
    auto& font = get_font("fonts/NewCM10-Regular.otf");
    auto shaped_glyphs_per_line = std::vector<std::vector<ShapedGlyph>>();
    auto later_strings = strings;
    later_strings.push_back("\n");
    for (int i = 0; i < ssize(later_strings); ++i) {
        while (true) {
            auto pos = later_strings[i].find_first_of('\n');
            if (pos == std::string_view::npos) break;
            auto earlier_strings = later_strings;
            earlier_strings.resize(i+1);
            earlier_strings[i] = earlier_strings[i].substr(0, pos);
            shaped_glyphs_per_line.push_back(shape_text(font, earlier_strings));
            for (int j = 0; j < i; ++j) {
                later_strings[j] = "";
            }
            later_strings[i] = later_strings[i].substr(pos+1);
        }
    }

    struct Intermediate {
        double x = 0;
        double y = 0;
        double width = 0;
        double height = 0;
        float texture_x = 0;
        float texture_y = 0;
        float texture_width = 0;
        float texture_height = 0;
        int group = -1;
    };

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

    auto y_plus = 0.0;
    const auto ascender = get_font_ascender(font);
    const auto descender = get_font_descender(font);

    for (auto& shaped_glyphs : shaped_glyphs_per_line) {
        auto positioned = std::vector<Intermediate>();
        auto x_min = double(INFINITY);
        auto x_max = double(-INFINITY);
        for (auto& glyph : shaped_glyphs) {
            auto& c = *glyph.glyph;
            x_min = std::min(x_min, glyph.x_pos + c.bearing_x);
            x_max = std::max(x_max, glyph.x_pos + c.bearing_x);
            x_min = std::min(x_min, glyph.x_pos + c.bearing_x + c.width);
            x_max = std::max(x_max, glyph.x_pos + c.bearing_x + c.width);
            y_mins[glyph.group_index] = std::min(
                y_mins[glyph.group_index], glyph.y_pos + descender + y_plus);
            y_maxs[glyph.group_index] = std::max(
                y_maxs[glyph.group_index], glyph.y_pos + ascender + y_plus);
            positioned.emplace_back(
                glyph.x_pos + c.bearing_x,
                c.bearing_y + glyph.y_pos + y_plus,
                c.width,
                -c.height,
                c.texture_x,
                c.texture_y,
                c.texture_width,
                c.texture_height,
                glyph.group_index
            );
        }
        const auto x_shift = -(x_min + x_max) / 2;
        for (auto& vertex : positioned) {
            vertex.x += x_shift;
            auto j = all_vertices[vertex.group].size();
            all_vertices[vertex.group].push_back({
                static_cast<float>(vertex.x),
                static_cast<float>(vertex.y),
                0, static_cast<float>(0 + j*4)
            });
            all_tvertices[vertex.group].push_back(
                    {vertex.texture_x, vertex.texture_y});
            all_vertices[vertex.group].push_back({
                static_cast<float>(vertex.x + vertex.width),
                static_cast<float>(vertex.y),
                0, static_cast<float>(1 + j*4)
            });
            all_tvertices[vertex.group].push_back(
                    {vertex.texture_x + vertex.texture_width,
                     vertex.texture_y});
            all_vertices[vertex.group].push_back({
                static_cast<float>(vertex.x),
                static_cast<float>(vertex.y + vertex.height),
                0, static_cast<float>(2 + j*4)
            });
            all_tvertices[vertex.group].push_back(
                    {vertex.texture_x,
                     vertex.texture_y + vertex.texture_height});
            all_vertices[vertex.group].push_back({
                static_cast<float>(vertex.x + vertex.width),
                static_cast<float>(vertex.y + vertex.height),
                0, static_cast<float>(3 + j*4)
            });
            all_tvertices[vertex.group].push_back(
                    {vertex.texture_x + vertex.texture_width,
                     vertex.texture_y + vertex.texture_height});
            all_indices[vertex.group].push_back(j + 0);
            all_indices[vertex.group].push_back(j + 1);
            all_indices[vertex.group].push_back(j + 2);
            all_indices[vertex.group].push_back(j + 2);
            all_indices[vertex.group].push_back(j + 1);
            all_indices[vertex.group].push_back(j + 3);
        }
        y_plus -= ascender - descender + newline_buff;
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
