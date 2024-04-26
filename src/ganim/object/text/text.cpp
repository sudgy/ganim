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
    auto last_letter = '\0';
    for (auto letter : string) {
        auto& c = get_character(font, letter);
        auto kerning = get_kerning(font, letter, last_letter);
        pos += kerning;
        last_letter = letter;
        vertices.push_back({
            static_cast<float>(pos + c.bearing_x),
            static_cast<float>(c.bearing_y),
            0, 0
        });
        tvertices.push_back({c.texture_x, c.texture_y});
        vertices.push_back({
            static_cast<float>(pos + c.bearing_x + c.width),
            static_cast<float>(c.bearing_y),
            0, 1
        });
        tvertices.push_back({c.texture_x + c.texture_width, c.texture_y});
        vertices.push_back({
            static_cast<float>(pos + c.bearing_x),
            static_cast<float>(c.bearing_y - c.height),
            0, 2
        });
        tvertices.push_back({c.texture_x, c.texture_y + c.texture_height});
        vertices.push_back({
            static_cast<float>(pos + c.bearing_x + c.width),
            static_cast<float>(c.bearing_y - c.height),
            0, 3
        });
        tvertices.push_back({c.texture_x + c.texture_width,
                c.texture_y + c.texture_height});
        indices.push_back(4*i + 0);
        indices.push_back(4*i + 1);
        indices.push_back(4*i + 2);
        indices.push_back(4*i + 2);
        indices.push_back(4*i + 1);
        indices.push_back(4*i + 3);
        pos += c.x_advance;
        ++i;
    }
    const auto x_shift = pos / 2;
    for (auto& v : vertices) {
        v.x -= x_shift;
    }
    set_vertices(std::move(vertices), std::move(indices));
    set_texture_vertices(std::move(tvertices));
    set_texture(get_text_texture());
}
