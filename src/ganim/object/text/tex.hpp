#ifndef GANIM_OBJECT_TEXT_TEX_HPP
#define GANIM_OBJECT_TEXT_TEX_HPP

#include "ganim/object/texture_shape.hpp"
#include "ganim/object/bases/group.hpp"
#include "dvi_reader.hpp"

namespace ganim {

namespace deprecated {class Character;}

class Tex : public Group, public DVIConsumer {
    public:
        explicit Tex(std::filesystem::path dvi_filename);
        template <typename... Ts>
        explicit Tex(Ts&&... tex_strings)
        requires((std::convertible_to<Ts, std::string> and ...) and
                sizeof...(tex_strings) > 0)
        : Tex(static_cast<const std::vector<std::string>&>(
                    std::vector<std::string>{tex_strings...})) {}
        Tex(const std::vector<std::string>& tex_strings);

        virtual int write_character(
            const DVIFont& font,
            std::uint32_t c,
            int h,
            int v
        ) override;
        virtual void draw_rect(
            int h,
            int v,
            int a,
            int b
        ) override;
        virtual void set_magnification(double mag) override
            {M_magnification = mag;}
        virtual void process_special(std::string_view special) override;
        void set_colors(const std::unordered_map<std::string, Color>& colors);

    private:
        double M_magnification = 0.0;
        struct tex_vertex {
            deprecated::Character* character = nullptr;
            double x = 0.0;
            double y = 0.0;
            double scale = 1.0;
        };
        struct rule {
            double x;
            double y;
            double width;
            double height;
        };
        std::vector<std::vector<tex_vertex>> M_vertices;
        std::vector<std::vector<rule>> M_rules;
        int M_current_section = 0;

        class TexPiece : public TextureShape<Shape> {
            public:
                using TextureShape<Shape>::TextureShape;
                virtual Box get_original_logical_bounding_box() const override;
                Box logical_bounding_box;
        };

        std::vector<ObjectPtr<TexPiece>> M_shapes;
        std::unordered_map<std::string, std::vector<int>> M_pieces_by_string;
        std::vector<std::string> M_tex_strings;
        double M_ascender = 0.0;
        double M_descender = 0.0;
};

inline ObjectPtr<Tex> make_tex(std::filesystem::path dvi_filename)
{
    return ObjectPtr<Tex>(dvi_filename);
}

template <typename... Ts>
ObjectPtr<Tex> make_tex(Ts&&... tex_strings)
{
    return ObjectPtr<Tex>(std::forward<Ts>(tex_strings)...);
}

inline ObjectPtr<Tex> make_tex(const std::vector<std::string>& tex_strings)
{
    return ObjectPtr<Tex>(tex_strings);
}

}

#endif
