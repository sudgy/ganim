#include "style.hpp"

#include <utility>

using namespace ganim;
using namespace ganim::gex;
using enum Style;

Style gex::get_superscript_style(Style style)
{
    switch (style) {
        case Display: return Script;
        case Text: return Script;
        case Script: return ScriptScript;
        case ScriptScript: return ScriptScript;
        case CrampedDisplay: return CrampedScript;
        case CrampedText: return CrampedScript;
        case CrampedScript: return CrampedScriptScript;
        case CrampedScriptScript: return CrampedScriptScript;
    }
    std::unreachable();
}

Style gex::get_subscript_style(Style style)
{
    switch (style) {
        case Display: return CrampedScript;
        case Text: return CrampedScript;
        case Script: return CrampedScriptScript;
        case ScriptScript: return CrampedScriptScript;
        case CrampedDisplay: return CrampedScript;
        case CrampedText: return CrampedScript;
        case CrampedScript: return CrampedScriptScript;
        case CrampedScriptScript: return CrampedScriptScript;
    }
    std::unreachable();
}

Style gex::get_numerator_style(Style style)
{
    switch (style) {
        case Display: return Text;
        case Text: return Script;
        case Script: return ScriptScript;
        case ScriptScript: return ScriptScript;
        case CrampedDisplay: return CrampedText;
        case CrampedText: return CrampedScript;
        case CrampedScript: return CrampedScriptScript;
        case CrampedScriptScript: return CrampedScriptScript;
    }
    std::unreachable();
}

Style gex::get_denominator_style(Style style)
{
    switch (style) {
        case Display: return CrampedText;
        case Text: return CrampedScript;
        case Script: return CrampedScriptScript;
        case ScriptScript: return CrampedScriptScript;
        case CrampedDisplay: return CrampedText;
        case CrampedText: return CrampedScript;
        case CrampedScript: return CrampedScriptScript;
        case CrampedScriptScript: return CrampedScriptScript;
    }
    std::unreachable();
}

Style gex::get_cramped_style(Style style)
{
    switch (style) {
        case Display: return CrampedDisplay;
        case Text: return CrampedText;
        case Script: return CrampedScript;
        case ScriptScript: return CrampedScriptScript;
        case CrampedDisplay: return CrampedDisplay;
        case CrampedText: return CrampedText;
        case CrampedScript: return CrampedScript;
        case CrampedScriptScript: return CrampedScriptScript;
    }
    std::unreachable();
}

double gex::get_style_scaling(Style style)
{
    switch (style) {
        case Display: return 1.0;
        case Text: return 1.0;
        case Script: return 0.7;
        case ScriptScript: return 0.5;
        case CrampedDisplay: return 1.0;
        case CrampedText: return 1.0;
        case CrampedScript: return 0.7;
        case CrampedScriptScript: return 0.5;
    }
    std::unreachable();
}

bool gex::is_script_styles(Style style)
{
    switch (style) {
        case Display: return false;
        case Text: return false;
        case Script: return true;
        case ScriptScript: return true;
        case CrampedDisplay: return false;
        case CrampedText: return false;
        case CrampedScript: return true;
        case CrampedScriptScript: return true;
    }
    std::unreachable();
}
