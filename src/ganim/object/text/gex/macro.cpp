#include "macro.hpp"

#include <ranges>

#include "preprocessor.hpp"
#include "macros.cpp"

using namespace ganim;
using namespace ganim::gex;

std::unique_ptr<Preprocessor> MacroStack::S_base_preprocessor;

MacroStack::MacroStack()
{
    if (!S_base_preprocessor and !S_making_base_frame) {
        S_base_frame[U"relax"] = {};
        S_base_frame[U"displaystyle"] =
            {{}, {{CommandToken(U"displaystyle", "displaystyle")}}, true};
        S_base_frame[U"textstyle"] =
            {{}, {{CommandToken(U"textstyle", "textstyle")}}, true};
        S_base_frame[U"scriptstyle"] =
            {{}, {{CommandToken(U"scriptstyle", "scriptstyle")}}, true};
        S_base_frame[U"scriptscriptstyle"] =
            {{}, {{CommandToken(U"scriptscriptstyle", "scriptscriptstyle")}},
                true};
        S_base_frame[U"mathaccent"] =
            {{}, {{CommandToken(U"mathaccent", "mathaccent")}}, true};
        S_base_frame[U"abovewithdelims"] =
            {{}, {{CommandToken(U"abovewithdelims", "abovewithdelims")}}, true};
        S_base_frame[U"mskip"] =
            {{}, {{CommandToken(U"mskip", "mskip")}}, true};
        S_base_frame[U"mathord"] =
            {{}, {{CommandToken(U"mathord", "mathord")}}, true};
        S_base_frame[U"mathop"] =
            {{}, {{CommandToken(U"mathop", "mathop")}}, true};
        S_base_frame[U"mathbin"] =
            {{}, {{CommandToken(U"mathbin", "mathbin")}}, true};
        S_base_frame[U"mathrel"] =
            {{}, {{CommandToken(U"mathrel", "mathrel")}}, true};
        S_base_frame[U"mathopen"] =
            {{}, {{CommandToken(U"mathopen", "mathopen")}}, true};
        S_base_frame[U"mathclose"] =
            {{}, {{CommandToken(U"mathclose", "mathclose")}}, true};
        S_base_frame[U"mathpunct"] =
            {{}, {{CommandToken(U"mathpunct", "mathpunct")}}, true};
        S_base_frame[U"mathinner"] =
            {{}, {{CommandToken(U"mathinner", "mathinner")}}, true};
        S_base_frame[U"radical"] =
            {{}, {{CommandToken(U"radical", "radical")}}, true};

        S_base_frame[U"{"] = {{}, {{CharacterToken(U'{',CategoryCode::Other)}}};
        S_base_frame[U"}"] = {{}, {{CharacterToken(U'}',CategoryCode::Other)}}};
        S_base_frame[U"$"] = {{}, {{CharacterToken(U'$',CategoryCode::Other)}}};
        S_base_frame[U"&"] = {{}, {{CharacterToken(U'&',CategoryCode::Other)}}};
        S_base_frame[U"#"] = {{}, {{CharacterToken(U'#',CategoryCode::Other)}}};
        S_base_frame[U"^"] = {{}, {{CharacterToken(U'^',CategoryCode::Other)}}};
        S_base_frame[U"_"] = {{}, {{CharacterToken(U'_',CategoryCode::Other)}}};
        S_base_frame[U"%"] = {{}, {{CharacterToken(U'%',CategoryCode::Other)}}};
        // No idea if this is the best way to implement this, but it should work
        S_base_frame[U" "] = {{}, {{CharacterToken(U' ',CategoryCode::Other)}}};

        // Special commands, when an empty token list is found it will try to
        // match up to one of these
        S_base_frame[U"def"] = {};
        S_base_frame[U"expandafter"] = {};
        S_making_base_frame = true;
        S_base_preprocessor = std::make_unique<Preprocessor>(false);
        for (auto s : macro_input) {
            add_base_macros(s);
        }
    }
    M_frames.emplace_back();
}

void MacroStack::push()
{
    M_frames.emplace_back();
}

void MacroStack::pop()
{
    M_frames.pop_back();
}

const Macro& MacroStack::get_macro(const std::u32string& name)
{
    for (auto& frame : std::views::reverse(M_frames)) {
        auto it = frame.find(name);
        if (it != frame.end()) {
            return it->second;
        }
    }
    auto it = S_base_frame.find(name);
    if (it != S_base_frame.end()) {
        return it->second;
    }
    throw std::runtime_error("Undefined control sequence");
}

void MacroStack::add_macro(const std::u32string& name, Macro macro)
{
    if (S_making_base_frame and M_frames.size() == 1) {
        S_base_frame[name] = std::move(macro);
    }
    else {
        M_frames.back()[name] = std::move(macro);
    }
}

void MacroStack::add_base_macros(std::string_view input)
{
    S_making_base_frame = true;
    S_base_preprocessor->process({input});
    S_making_base_frame = false;
}
