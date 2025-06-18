#ifndef GANIM_OBJECT_TEXT_GEX_HPP
#define GANIM_OBJECT_TEXT_GEX_HPP

#include "text_base.hpp"

namespace ganim {

class Gex : public TextBase {
    public:
        template <typename... Ts>
        explicit Gex(Ts&&... tex_strings)
        requires((std::convertible_to<Ts, std::string_view> and ...) and
                sizeof...(tex_strings) > 0)
        : Gex(static_cast<const std::vector<std::string_view>&>(
            std::vector<std::string_view>{std::forward<Ts>(tex_strings)...})) {}
        Gex(const std::vector<std::string_view>& tex_strings);

        //void set_colors(const std::unordered_map<std::string, Color>& colors);

        ObjectPtr<Gex> polymorphic_copy() const;

    private:
        virtual Gex* polymorphic_copy_impl() const override;

        virtual std::vector<PositionedGlyph> get_glyphs(
                const std::vector<std::string_view>& strings) override;

        Font* M_font = nullptr;
};

template <typename... Ts>
ObjectPtr<Gex> make_gex(Ts&&... tex_strings)
{
    return ObjectPtr<Gex>(std::forward<Ts>(tex_strings)...);
}

inline ObjectPtr<Gex> make_gex(const std::vector<std::string_view>& tex_strings)
{
    return ObjectPtr<Gex>(tex_strings);
}

}

#endif
