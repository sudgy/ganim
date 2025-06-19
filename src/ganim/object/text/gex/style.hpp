#ifndef GANIM_GEX_STYLE_HPP
#define GANIM_GEX_STYLE_HPP

namespace ganim::gex {
    enum class Style {
        Display,
        Text,
        Script,
        ScriptScript,
        CrampedDisplay,
        CrampedText,
        CrampedScript,
        CrampedScriptScript
    };
    Style get_superscript_style(Style style);
    Style get_subscript_style(Style style);
    Style get_numerator_style(Style style);
    Style get_denominator_style(Style style);
    Style get_cramped_style(Style style);
    double get_style_scaling(Style style);
}

#endif
