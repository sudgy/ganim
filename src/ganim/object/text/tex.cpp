#include "tex.hpp"

#include <fstream>

#include "character.hpp"

using namespace ganim;

namespace {
    std::filesystem::path create_dvi(
        const std::vector<std::string>& tex_strings
    )
    {
        std::filesystem::create_directories("ganim_files/tex/");
        auto tex_file = std::ofstream("ganim_files/tex/ganim.tex");
        tex_file <<
R"(
\documentclass[preview]{standalone}

\usepackage{amsmath}

\begin{document}
\begin{align*}
)";
        for (auto& s : tex_strings) tex_file << s;
        tex_file <<
R"(
\end{align*}
\end{document}
)";
        tex_file.close();
        if (std::system("latex "
                "--output-directory=ganim_files/tex/ "
                "-halt-on-error "
                "ganim_files/tex/ganim.tex "
// Please tell me there's a better way to do this
#ifdef _WIN32
                ">nul 2>nul"
#else
                ">/dev/null 2>/dev/null"
#endif
        )) {
            throw std::runtime_error("LaTeX failed to compile");
        }
        return "ganim_files/tex/ganim.dvi";
    }
}

Tex::Tex(const std::vector<std::string>& tex_strings)
: Tex(create_dvi(tex_strings)) {}

Tex::Tex(std::filesystem::path dvi_filename)
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
    for (auto [c, x, y, s] : M_vertices) {
        x_min = std::min(x_min, x + c->bearing_x);
        x_max = std::max(x_max, x + c->bearing_x + c->width);
        y_min = std::min(y_min, y + c->bearing_y - c->height);
        y_max = std::max(y_max, y + c->bearing_y);
        vertices.push_back({
            static_cast<float>(x + c->bearing_x * s),
            static_cast<float>(y + c->bearing_y * s)
        });
        tvertices.push_back({c->texture_x, c->texture_y});
        vertices.push_back({
            static_cast<float>(x + (c->bearing_x + c->width) * s),
            static_cast<float>(y + c->bearing_y * s)
        });
        tvertices.push_back({c->texture_x + c->texture_width, c->texture_y});
        vertices.push_back({
            static_cast<float>(x + c->bearing_x * s),
            static_cast<float>(y + (c->bearing_y - c->height) * s)
        });
        tvertices.push_back({c->texture_x, c->texture_y + c->texture_height});
        vertices.push_back({
            static_cast<float>(x + (c->bearing_x + c->width) * s),
            static_cast<float>(y + (c->bearing_y - c->height) * s)
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
    for (auto [x, y, width, height] : M_rules) {
        const auto td = 1.0f / GC_default_text_texture_size / 2;
        x_min = std::min(x_min, x);
        x_max = std::max(x_max, x + width);
        y_min = std::min(y_min, y);
        y_max = std::max(y_max, y + height);
        vertices.push_back({
            static_cast<float>(x),
            static_cast<float>(y)
        });
        tvertices.push_back({0, 0});
        vertices.push_back({
            static_cast<float>(x + width),
            static_cast<float>(y)
        });
        tvertices.push_back({td, 0});
        vertices.push_back({
            static_cast<float>(x),
            static_cast<float>(y + height)
        });
        tvertices.push_back({0, td});
        vertices.push_back({
            static_cast<float>(x + width),
            static_cast<float>(y + height)
        });
        tvertices.push_back({td, td});
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
    auto scale = M_magnification * 1e-5 / 2.54 * 72 / 10
        * get_font_pem(font2);
    M_vertices.emplace_back(&character, h * scale, -v * scale, font.size / 10.0);
    return character.x_advance_em * font.size / 72 * 2.54 * 1e5
        / M_magnification;
}

void Tex::draw_rect(
    int h,
    int v,
    int a,
    int b
)
{
    // TODO: Figure out how to get the pem value in here
    auto scale = M_magnification * 1e-5 / 2.54 * 72 / 10;
    M_rules.emplace_back(h * scale, -v * scale, b * scale, a * scale);
}
