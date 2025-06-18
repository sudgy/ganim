#ifndef GANIM_OBJECT_TEXT_HPP
#define GANIM_OBJECT_TEXT_HPP

/** @file
 * @brief Contains the @ref ganim::Text class
 */

#include "text_base.hpp"

namespace ganim {
    struct TextArgs {
        double newline_buff = 0.0;
        std::string_view font_filename = "fonts/NewCM10-Regular.otf";
        int font_pixel_size = 128;
    };
    class Text : public TextBase {
        public:
            template <typename... Ts>
            explicit Text(Ts&&... strings)
                requires((std::convertible_to<Ts, std::string_view> and ...) and
                        sizeof...(strings) > 0)
            : Text(static_cast<const std::vector<std::string_view>&>(
                std::vector<std::string_view>{std::forward<Ts>(strings)...})) {}
            explicit Text(const std::vector<std::string_view>& strings);

            template <typename... Ts>
            explicit Text(TextArgs args, Ts&&... strings)
                requires((std::convertible_to<Ts, std::string_view> and ...) and
                        sizeof...(strings) > 0)
            : Text(args,
                static_cast<const std::vector<std::string_view>&>(
                std::vector<std::string_view>{std::forward<Ts>(strings)...})
            ) {}
            explicit Text(
                TextArgs args,
                const std::vector<std::string_view>& strings
            );

            ObjectPtr<Text> polymorphic_copy() const;

        private:
            virtual Text* polymorphic_copy_impl() const override;

            virtual std::vector<PositionedGlyph> get_glyphs(
                    const std::vector<std::string_view>& strings) override;
            Font* M_font = nullptr;
            double M_newline_buff = 0.0;
    };

    template <typename... Ts>
    ObjectPtr<Text> make_text(Ts&&... tex_strings)
        requires(std::convertible_to<Ts, std::string_view> and ...)
    {
        return ObjectPtr<Text>(std::forward<Ts>(tex_strings)...);
    }

    inline ObjectPtr<Text> make_text(
        const std::vector<std::string_view>& tex_strings
    )
    {
        return ObjectPtr<Text>(tex_strings);
    }
    template <typename... Ts>
    ObjectPtr<Text> make_text(TextArgs args, Ts&&... tex_strings)
        requires(std::convertible_to<Ts, std::string_view> and ...)
    {
        return ObjectPtr<Text>(args, std::forward<Ts>(tex_strings)...);
    }

    inline ObjectPtr<Text> make_text(
        TextArgs args,
        const std::vector<std::string_view>& tex_strings
    )
    {
        return ObjectPtr<Text>(args, tex_strings);
    }
}

#endif
