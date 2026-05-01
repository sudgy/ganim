#ifndef GANIM_OBJECT_TEXT_GEX_HPP
#define GANIM_OBJECT_TEXT_GEX_HPP

#include "text_base.hpp"

namespace ganim {

class Gex : public TextBase {
    public:
        template <typename... Ts>
        explicit Gex(bool math, int pixel_size, Ts&&... tex_strings)
        requires((std::convertible_to<Ts, std::string_view> and ...) and
                sizeof...(tex_strings) > 0)
        : Gex(
            math,
            pixel_size,
            static_cast<const std::vector<std::string_view>&>(
                std::vector<std::string_view>{std::forward<Ts>(tex_strings)...}
            )
        ) {}
        Gex(
            bool math,
            int pixel_size,
            const std::vector<std::string_view>& tex_strings
        );

        void set_colors(const std::unordered_map<std::string, Color>& colors);

        ObjectPtr<Gex> copy() const;

        double get_axis_y() const;

        inline thread_local static auto default_color_map
            = std::unordered_map<std::string, Color>();
        inline thread_local static auto default_scale = 1.0;

    private:
        virtual Gex* copy_impl() const override;

        virtual std::vector<Glyph> get_glyphs(
                const std::vector<std::string_view>& strings) override;

        Font* M_font = nullptr;
        std::vector<std::string> M_tex_strings;
        bool M_math = true;
        int M_pixel_size = 128;
};

template <typename... Ts>
ObjectPtr<Gex> make_gex(Ts&&... tex_strings)
{
    return ObjectPtr<Gex>(true, 128, std::forward<Ts>(tex_strings)...);
}

inline ObjectPtr<Gex> make_gex(const std::vector<std::string_view>& tex_strings)
{
    return ObjectPtr<Gex>(true, 128, tex_strings);
}

template <typename... Ts>
ObjectPtr<Gex> make_gext(Ts&&... tex_strings)
{
    return ObjectPtr<Gex>(false, 128, std::forward<Ts>(tex_strings)...);
}

inline ObjectPtr<Gex> make_gext(const std::vector<std::string_view>& tex_strings)
{
    return ObjectPtr<Gex>(false, 128, tex_strings);
}

template <typename... Ts>
ObjectPtr<Gex> make_scaled_gex(int pixel_size, Ts&&... tex_strings)
{
    return ObjectPtr<Gex>(true, pixel_size, std::forward<Ts>(tex_strings)...);
}

inline ObjectPtr<Gex> make_scaled_gex(
    int pixel_size,
    const std::vector<std::string_view>& tex_strings
)
{
    return ObjectPtr<Gex>(true, pixel_size, tex_strings);
}

template <typename... Ts>
ObjectPtr<Gex> make_scaled_gext(int pixel_size, Ts&&... tex_strings)
{
    return ObjectPtr<Gex>(false, pixel_size, std::forward<Ts>(tex_strings)...);
}

inline ObjectPtr<Gex> make_scaled_gext(
    int pixel_size,
    const std::vector<std::string_view>& tex_strings
)
{
    return ObjectPtr<Gex>(false, pixel_size, tex_strings);
}

}

#endif
