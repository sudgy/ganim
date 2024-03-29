#ifndef GANIM_OBJECT_TEXT_TEX_HPP
#define GANIM_OBJECT_TEXT_TEX_HPP

#include "ganim/object/texture_shape.hpp"
#include "dvi_reader.hpp"

namespace ganim {

class Character;

class Tex : public TextureShape<Shape>, public DVIConsumer {
    public:
        explicit Tex(std::filesystem::path dvi_filename);
        template <typename... Ts>
        explicit Tex(Ts&&... tex_strings)
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

    private:
        double M_magnification = 0.0;
        struct tex_vertex {
            Character* character = nullptr;
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
        std::vector<tex_vertex> M_vertices;
        std::vector<rule> M_rules;
};

}

#endif
