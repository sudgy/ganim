#include "tex.hpp"

#include "character.hpp"

using namespace ganim;

Tex::Tex(std::string_view dvi_filename)
{
    read_dvi(dvi_filename, *this);
    auto vertices = std::vector<Vertex>();
    auto indices = std::vector<unsigned>();
    auto tvertices = std::vector<TextureVertex>();
    auto i = 0;
    double x_min = INFINITY;
    double x_max = -INFINITY;
    double y_min = INFINITY;
    double y_max = -INFINITY;
    for (auto [c, x, y] : M_vertices) {
        x_min = std::min(x_min, x + c->bearing_x);
        x_max = std::max(x_max, x + c->bearing_x + c->width);
        y_min = std::min(y_min, y + c->bearing_y - c->height);
        y_max = std::max(y_max, y + c->bearing_y);
        vertices.push_back({
            static_cast<float>(x + c->bearing_x),
            static_cast<float>(y + c->bearing_y),
            0
        });
        tvertices.push_back({c->texture_x, c->texture_y});
        vertices.push_back({
            static_cast<float>(x + c->bearing_x + c->width),
            static_cast<float>(y + c->bearing_y),
            0
        });
        tvertices.push_back({c->texture_x + c->texture_width, c->texture_y});
        vertices.push_back({
            static_cast<float>(x + c->bearing_x),
            static_cast<float>(y + c->bearing_y - c->height),
            0
        });
        tvertices.push_back({c->texture_x, c->texture_y + c->texture_height});
        vertices.push_back({
            static_cast<float>(x + c->bearing_x + c->width),
            static_cast<float>(y + c->bearing_y - c->height),
            0
        });
        tvertices.push_back({c->texture_x + c->texture_width,
                c->texture_y + c->texture_height});
        indices.push_back(4*i + 0);
        indices.push_back(4*i + 1);
        indices.push_back(4*i + 2);
        indices.push_back(4*i + 2);
        indices.push_back(4*i + 1);
        indices.push_back(4*i + 3);
        ++i;
    }
    const auto x_shift = (x_min + x_max) / 2;
    const auto y_shift = (y_min + y_max) / 2;
    for (auto& v : vertices) {
        v.x -= x_shift;
        v.y -= y_shift;
    }
    set_vertices(std::move(vertices), std::move(indices));
    set_texture_vertices(std::move(tvertices));
    set_texture(get_text_texture());
}

int Tex::write_character(
    const DVIFont& font,
    std::uint32_t c,
    int h,
    int v
)
{
    auto filename = font.name;
    // TODO: Figure out how this works better
    if (font.system) {
        filename = "/usr/share/texmf-dist/fonts/type1/public/amsfonts/cm/"
            + filename + ".pfb";
    }
    auto& font2 = get_font(filename);
    auto& character = get_character(font2, c);
    auto scale = M_magnification * 1e-5 / 2.54 * 72 / font.size
        * get_font_pem(font2);
    M_vertices.emplace_back(&character, h * scale, v * scale);
    return character.x_advance_em * font.size / 72 * 2.54 * 1e5
        / M_magnification;
}
