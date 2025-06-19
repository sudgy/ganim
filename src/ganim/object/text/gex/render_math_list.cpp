#include "render_math_list.hpp"

#include "math_letter_map.hpp"

using namespace ganim;
using namespace ganim::gex;

namespace {
    // Experimentally, the distance from the midline to the baseline is 0.265625
    //void vcenter(Box& box)
    //{

    //}
    void do_preprocessing(MathList& list)
    {
        using enum AtomType;
        auto last_atom = (Atom*)nullptr;
        auto last_type = AtomType(-1);
        for (int i = 0; i < ssize(list); ++i) {
            auto& noad = list[i];
            if (auto atom = std::get_if<Atom>(&noad.value)) {
                // Step 5
                if (atom->type == Bin) {
                    if (last_type == AtomType(-1) or
                        last_type == Bin or
                        last_type == Op or
                        last_type == Rel or
                        last_type == Open or
                        last_type == Punct)
                    {
                        atom->type = Ord;
                    }
                }
                // Step 6
                else if ((atom->type == Rel or
                         atom->type == Close or
                         atom->type == Punct) and last_type == Bin)
                {
                    last_atom->type = Ord;
                }
                last_type = atom->type;
                last_atom = atom;
            }
        }
        if (last_type == Bin) {
            last_atom->type = Ord;
        }
    }
    std::vector<Atom> do_rendering(MathList& list, Font& font, Style style)
    {
        auto scaling = get_style_scaling(style);
        using enum AtomType;
        auto rendered_list = std::vector<Atom>();
        auto ord_start = -1;
        auto render_ords = [&](int start, int end) {
            auto text = std::vector<std::pair<std::u32string, int>>();
            for (int i = start; i < end; ++i) {
                auto& atom = get<Atom>(list[i].value);
                auto& atom_symbol = get<AtomFieldSymbol>(atom.nucleus.value);
                auto it = math_letter_map.find(atom_symbol.codepoint);
                auto codepoint = it == math_letter_map.end()
                    ? atom_symbol.codepoint
                    : it->second;
                text.emplace_back(std::u32string(1, codepoint), list[i].group);
            }
            auto glyphs = shape_text_manual_groups(font, text);
            auto box = box_from_glyphs(glyphs);
            scale_box(box, scaling);
            auto new_atom = Atom(box, Ord, AtomField(AtomFieldBox(), box));
            rendered_list.push_back(new_atom);
        };
        auto render_atom = [&](auto& atom, int group) {
            if (auto atom_symbol = get_if<AtomFieldSymbol>(&atom.nucleus.value)) {
                auto glyphs = shape_text_manual_groups(
                    font,
                    {{std::u32string(1, atom_symbol->codepoint), group}}
                );
                atom.nucleus.box = box_from_glyphs(glyphs);
                scale_box(atom.nucleus.box, scaling);
                atom.box = atom.nucleus.box;
                rendered_list.push_back(atom);
            }
        };
        auto is_symbol_ord = [&](auto& atom) {
            return atom->type == Ord and
                std::holds_alternative<AtomFieldSymbol>(atom->nucleus.value);
        };
        for (int i = 0; i < ssize(list); ++i) {
            auto& noad = list[i];
            if (auto atom = std::get_if<Atom>(&noad.value)) {
                if (is_symbol_ord(atom) and ord_start == -1) ord_start = i;
                else if (!is_symbol_ord(atom)) {
                    if (ord_start != -1) {
                        render_ords(ord_start, i);
                        ord_start = -1;
                    }
                    render_atom(*atom, noad.group);
                }
            }
        }
        if (ord_start != -1) {
            render_ords(ord_start, ssize(list));
        }
        return rendered_list;
    }
    Box do_combine(std::vector<Atom>& rendered_list, Font& font, Style style)
    {
        auto scaling = get_style_scaling(style);
        const auto em = get_font_em(font);
        auto result_boxes = std::vector<Box>();
        for (int i = 0; i < ssize(rendered_list); ++i) {
            result_boxes.push_back(rendered_list[i].box);
            if (i + 1 < ssize(rendered_list)) {
                auto spacing = get_atom_spacing(
                    rendered_list[i].type,
                    rendered_list[i+1].type
                );
                result_boxes.push_back(
                    Box(scaling*em*spacing.first/18.0, 0, 0, {}));
            }
        }
        return combine_boxes_horizontally(result_boxes);
    }
}

Box gex::render_math_list(MathList list, Style style)
{
    using enum AtomType;
    do_preprocessing(list);
    auto& font = get_font("fonts/NewCMMath-Regular.otf");
    auto rendered_list = do_rendering(list, font, style);
    return do_combine(rendered_list, font, style);
}
