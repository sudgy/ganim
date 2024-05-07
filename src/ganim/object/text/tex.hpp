#ifndef GANIM_OBJECT_TEXT_TEX_HPP
#define GANIM_OBJECT_TEXT_TEX_HPP

#include "ganim/object/texture_shape.hpp"
#include "ganim/object/cluster.hpp"
#include "dvi_reader.hpp"

namespace ganim {

class Character;

class Tex : public Cluster, public DVIConsumer {
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
        virtual void process_special(std::string_view special) override;
        void set_colors(const std::unordered_map<std::string, Color>& colors);

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
        std::vector<std::vector<tex_vertex>> M_vertices;
        std::vector<std::vector<rule>> M_rules;
        int M_current_section = 0;

        std::vector<TextureShape<Shape>> M_shapes;
        std::unordered_map<std::string, std::vector<int>> M_pieces_by_string;
};

}

#endif
