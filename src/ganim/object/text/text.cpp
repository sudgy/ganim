#include "text.hpp"

#include "character.hpp"

using namespace ganim;

Text::Text(std::string_view string)
{
    auto pos = 0.0;
    auto& font = get_font("/usr/share/texmf-dist/fonts/type1/public/amsfonts/cm/cmr10.pfb");
    auto vertices = std::vector<Vertex>();
    auto indices = std::vector<unsigned>();
    auto tvertices = std::vector<TextureVertex>();
    auto i = 0;
    auto start_i = 0;
    auto last_letter = '\0';
    auto y_plus = 0.0;
    auto y_min = double(INFINITY);
    auto y_max = double(-INFINITY);
    const auto ascender = get_font_ascender(font);
    const auto descender = get_font_descender(font);
    for (auto letter : string) {
        auto& c = get_character(font, letter);
        auto kerning = get_kerning(font, letter, last_letter);
        last_letter = letter;
        if (letter == '\n') {
            const auto x_shift = pos / 2;
            for (int j = 4*start_i; j < 4*i; ++j) {
                vertices[j].x -= x_shift;
            }
            pos = 0;
            y_plus -= 1.1; // TODO: Make this way better
            start_i = i;
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
    const auto x_shift = pos / 2;
    for (int j = 4*start_i; j < ssize(vertices); ++j) {
        vertices[j].x -= x_shift;
    }
    set_vertices(std::move(vertices), std::move(indices));
    set_texture_vertices(std::move(tvertices));
    set_texture(get_text_texture());
    auto true_bounding_box = get_true_bounding_box();
    using namespace vga2;
    auto x_min = pga2_to_vga2(
            true_bounding_box.get_lower_left()).blade_project<e1>();
    auto x_max = pga2_to_vga2(
            true_bounding_box.get_upper_right()).blade_project<e1>();
    M_logical_bounding_box = Box(x_min*e1 + y_min*e2, x_max*e1 + y_max*e2);
}

Box Text::get_original_logical_bounding_box() const
{
    return M_logical_bounding_box;
}
