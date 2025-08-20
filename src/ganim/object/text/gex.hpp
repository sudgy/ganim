#ifndef GANIM_OBJECT_TEXT_GEX_HPP
#define GANIM_OBJECT_TEXT_GEX_HPP

#include "text_base.hpp"

namespace ganim {

class Gex : public TextBase {
    public:
        template <typename... Ts>
        explicit Gex(bool math, Ts&&... tex_strings)
        requires((std::convertible_to<Ts, std::string_view> and ...) and
                sizeof...(tex_strings) > 0)
        : Gex(math, static_cast<const std::vector<std::string_view>&>(
            std::vector<std::string_view>{std::forward<Ts>(tex_strings)...})) {}
        Gex(bool math, const std::vector<std::string_view>& tex_strings);

        void set_colors(const std::unordered_map<std::string, Color>& colors);

        ObjectPtr<Gex> polymorphic_copy() const;

    private:
        virtual Gex* polymorphic_copy_impl() const override;

        virtual std::vector<PositionedGlyph> get_glyphs(
                const std::vector<std::string_view>& strings) override;

        Font* M_font = nullptr;
        std::vector<std::string> M_tex_strings;
        bool M_math = true;
};

template <typename... Ts>
ObjectPtr<Gex> make_gex(Ts&&... tex_strings)
{
    return ObjectPtr<Gex>(true, std::forward<Ts>(tex_strings)...);
}

inline ObjectPtr<Gex> make_gex(const std::vector<std::string_view>& tex_strings)
{
    return ObjectPtr<Gex>(true, tex_strings);
}

template <typename... Ts>
ObjectPtr<Gex> make_gext(Ts&&... tex_strings)
{
    return ObjectPtr<Gex>(false, std::forward<Ts>(tex_strings)...);
}

inline ObjectPtr<Gex> make_gext(const std::vector<std::string_view>& tex_strings)
{
    return ObjectPtr<Gex>(false, tex_strings);
}

}

#endif
