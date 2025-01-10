#include "text.hpp"

#include "character.hpp"

using namespace ganim;

Text::Text(const std::vector<std::string_view>& strings)
{
    set_draw_subobject_ratio(0.2);
    using Vertex = Shape::Vertex;
    auto pos = 0.0;
    auto& font = get_font("/usr/share/texmf-dist/fonts/type1/public/amsfonts/cm/cmr10.pfb");
    auto all_vertices = std::vector<std::vector<Vertex>>();
    auto all_indices = std::vector<std::vector<unsigned>>();
    auto all_tvertices = std::vector<std::vector<TextureVertex>>();
    all_vertices.resize(strings.size());
    all_indices.resize(strings.size());
    all_tvertices.resize(strings.size());
    auto i = 0;
    auto start_i = 0;
    auto start_n = 0;
    auto last_letter = '\0';
    auto y_plus = 0.0;
    auto y_mins = std::vector<double>();
    auto y_maxs = std::vector<double>();
    const auto ascender = get_font_ascender(font);
    const auto descender = get_font_descender(font);
    for (int n = 0; n < ssize(strings); ++n) {
        auto& string = strings[n];
        auto& vertices = all_vertices[n];
        auto& indices = all_indices[n];
        auto& tvertices = all_tvertices[n];
        i = 0;
        auto& y_min = y_mins.emplace_back(INFINITY);
        auto& y_max = y_maxs.emplace_back(-INFINITY);
        for (auto letter : string) {
            auto& c = get_character(font, letter);
            auto kerning = get_kerning(font, letter, last_letter);
            last_letter = letter;
            if (letter == '\n') {
                const auto x_shift = pos / 2;
                for (int m = start_n; m < n; ++m) {
                    auto start = m == start_n ? 4*start_i : 0;
                    auto end = m == n - 1 ? 4*i : ssize(vertices);
                    for (int j = start; j < end; ++j) {
                        all_vertices[m][j].x -= x_shift;
                    }
                }
                pos = 0;
                y_plus -= 1.1; // TODO: Make this way better
                start_i = i;
                start_n = n;
                continue;
            }
            if (letter != ' ') {
                y_min = std::min(y_min, y_plus + descender);
                y_max = std::max(y_max, y_plus + ascender);
                pos += kerning;
                vertices.push_back({
                    static_cast<float>(pos + c.bearing_x),
                    static_cast<float>(c.bearing_y + y_plus),
                    0, static_cast<float>(0 + i*4)
                });
                tvertices.push_back({c.texture_x, c.texture_y});
                vertices.push_back({
                    static_cast<float>(pos + c.bearing_x + c.width),
                    static_cast<float>(c.bearing_y + y_plus),
                    0, static_cast<float>(1 + i*4)
                });
                tvertices.push_back({c.texture_x + c.texture_width, c.texture_y});
                vertices.push_back({
                    static_cast<float>(pos + c.bearing_x),
                    static_cast<float>(c.bearing_y - c.height + y_plus),
                    0, static_cast<float>(2 + i*4)
                });
                tvertices.push_back({c.texture_x, c.texture_y + c.texture_height});
                vertices.push_back({
                    static_cast<float>(pos + c.bearing_x + c.width),
                    static_cast<float>(c.bearing_y - c.height + y_plus),
                    0, static_cast<float>(3 + i*4)
                });
                tvertices.push_back({c.texture_x + c.texture_width,
                        c.texture_y + c.texture_height});
                indices.push_back(4*i + 0);
                indices.push_back(4*i + 1);
                indices.push_back(4*i + 2);
                indices.push_back(4*i + 2);
                indices.push_back(4*i + 1);
                indices.push_back(4*i + 3);
                ++i;
            }
            pos += c.x_advance;
        }
    }
    const auto x_shift = pos / 2;
    for (int m = start_n; m < ssize(strings); ++m) {
        auto start = m == start_n ? 4*start_i : 0;
        for (int j = start; j < ssize(all_vertices[m]); ++j) {
            all_vertices[m][j].x -= x_shift;
        }
    }
    auto text_texture = get_text_texture();
    M_pieces.reserve(ssize(strings));
    for (int n = 0; n < ssize(strings); ++n) {
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
