#include "render_math_list.hpp"

#include <algorithm>

#include "math_letter_map.hpp"

using namespace ganim;
using namespace ganim::gex;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };

    void check_style_change(CommandNoad& command, Style& style, double& scaling)
    {
        if (command.command == "displaystyle") {
            style = Style::Display;
            scaling = get_style_scaling(style);
        }
        else if (command.command == "textstyle") {
            style = Style::Text;
            scaling = get_style_scaling(style);
        }
        else if (command.command == "scriptstyle") {
            style = Style::Script;
            scaling = get_style_scaling(style);
        }
        else if (command.command == "scriptscriptstyle") {
            style = Style::ScriptScript;
            scaling = get_style_scaling(style);
        }
    }

    class Processor {
        public:
            Processor(MathList& list) :
                M_list(list),
                M_font(get_font("fonts/NewCMMath-Regular.otf")) {}
            void do_preprocessing();
            void do_rendering(Style style);
            Box do_combine(Style style);

        private:
            void render_atom(Atom& atom, Style style);
            void render_atom_symbol(Atom& atom, AtomSymbol& symbol,Style style);
            void render_atom_list(Atom& atom, AtomList& list, Style style);
            void render_atom_script(Atom& atom, Style style);

            MathList& M_list;
            MathList M_rendered_list;
            Font& M_font;
    };
}

void Processor::do_preprocessing()
{
    using enum AtomType;
    auto last_atom = (Atom*)nullptr;
    auto last_type = AtomType(-1);
    for (int i = 0; i < ssize(M_list); ++i) {
        auto& noad = M_list[i];
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

void Processor::do_rendering(Style style)
{
    auto scaling = get_style_scaling(style);
    using enum AtomType;
    auto ord_start = -1;
    auto render_ords = [&](int start, int end) {
        auto text = std::vector<std::pair<std::u32string, int>>();
        for (int i = start; i < end; ++i) {
            auto& atom = get<Atom>(M_list[i].value);
            auto& atom_symbol = get<AtomSymbol>(atom.value);
            auto it = math_letter_map.find(atom_symbol.codepoint);
            auto codepoint = it == math_letter_map.end()
                ? atom_symbol.codepoint
                : it->second;
            text.emplace_back(std::u32string(1, codepoint), atom_symbol.group);
        }
        auto glyphs = shape_text_manual_groups(M_font, text);
        auto box = box_from_glyphs(glyphs);
        scale_box(box, scaling);
        auto new_atom = Atom(box, Ord, AtomBox());
        M_rendered_list.push_back(Noad(std::move(new_atom)));
    };
    auto is_symbol_ord = [&](auto& noad) {
        if (auto atom = std::get_if<Atom>(&noad.value)) {
            return atom->type == Ord and
                std::holds_alternative<AtomSymbol>(atom->value);
        }
        return false;
    };
    for (int i = 0; i < ssize(M_list); ++i) {
        auto& noad = M_list[i];
        if (is_symbol_ord(noad) and ord_start == -1) ord_start = i;
        else if (!is_symbol_ord(noad)) {
            if (ord_start != -1) {
                render_ords(ord_start, i);
                ord_start = -1;
            }
            if (auto atom = std::get_if<Atom>(&noad.value)) {
                render_atom(*atom, style);
                M_rendered_list.push_back(Noad(*atom));
            }
            else if (auto command = std::get_if<CommandNoad>(&noad.value)) {
                check_style_change(*command, style, scaling);
                M_rendered_list.push_back(noad);
            }
        }
    }
    if (ord_start != -1) {
        render_ords(ord_start, ssize(M_list));
    }
}

void Processor::render_atom(Atom& atom, Style style)
{
    std::visit(overloaded{
        [&](AtomSymbol& tok) {render_atom_symbol(atom, tok, style);},
        [&](AtomList& tok) {render_atom_list(atom, tok, style);},
        [&](AtomSubscript&)
            {render_atom_script(atom, style);},
        [&](AtomSuperscript&)
            {render_atom_script(atom, style);},
        [&](AtomSubsuperscript&)
            {render_atom_script(atom, style);},
        [&](auto&) {}
    }, atom.value);
}

void Processor::render_atom_symbol(Atom& atom, AtomSymbol& symbol, Style style)
{
    auto it = math_letter_map.find(symbol.codepoint);
    auto codepoint = it == math_letter_map.end()
        ? symbol.codepoint
        : it->second;
    auto scaling = get_style_scaling(style);
    auto glyphs = shape_text_manual_groups(
        M_font,
        {{std::u32string(1, codepoint), symbol.group}}
    );
    atom.box = box_from_glyphs(glyphs);
    scale_box(atom.box, scaling);
}

void Processor::render_atom_list(Atom& atom, AtomList& list, Style style)
{
    atom.box = render_math_list(list.list, style);
    atom.value = AtomBox();
}

void Processor::render_atom_script(
    Atom& atom,
    Style style
)
{
    Atom* nucleus = nullptr;
    Atom* subscript = nullptr;
    Atom* superscript = nullptr;
    if (auto atom2 = get_if<AtomSubscript>(&atom.value)) {
        nucleus = atom2->nucleus.get();
        subscript = atom2->subscript.get();
    }
    else if (auto atom2 = get_if<AtomSuperscript>(&atom.value)) {
        nucleus = atom2->nucleus.get();
        superscript = atom2->superscript.get();
    }
    else if (auto atom2 = get_if<AtomSubsuperscript>(&atom.value)) {
        nucleus = atom2->nucleus.get();
        subscript = atom2->subscript.get();
        superscript = atom2->superscript.get();
    }
    auto substyle = get_subscript_style(style);
    auto superstyle = get_superscript_style(style);
    auto subscaling = get_style_scaling(substyle);
    auto superscaling = get_style_scaling(superstyle);

    if (nucleus) {
        render_atom(*nucleus, style);
    }
    if (subscript) {
        render_atom(*subscript, substyle);
    }
    if (superscript) {
        render_atom(*superscript, superstyle);
    }

    const double scriptspace = get_font_em(M_font) / 20.0;
    const double σ5 = get_font_x_height(M_font);
    const double σ13 = get_font_sup1(M_font);
    const double σ14 = get_font_sup2(M_font);
    const double σ15 = get_font_sup3(M_font);
    const double σ16 = get_font_sub1(M_font);
    const double σ17 = get_font_sub2(M_font);
    const double σ18 = get_font_sup_drop(M_font);
    const double σ19 = get_font_sub_drop(M_font);
    const auto θ = get_font_default_rule_thickness(M_font);

    double u = 0.0;
    double v = 0.0;
    if (!holds_alternative<AtomSymbol>(nucleus->value)) {
        double h = nucleus->box.height;
        double d = nucleus->box.depth;
        double q = σ18 * superscaling;
        double r = σ19 * subscaling;
        u = h - q;
        v = d + r;
    }
    if (!superscript) {
        auto& x = subscript->box;
        horizontal_shift_box(x, scriptspace);
        x.width += scriptspace*2.5;
        vertical_shift_box(x, -std::max({
            v,
            σ16,
            x.height - std::abs(σ5) * 4.0/5.0
        }));
        atom.box = combine_boxes_horizontally({nucleus->box, x});
    }
    else {
        auto& x = superscript->box;
        horizontal_shift_box(x, scriptspace);
        x.width += scriptspace*2.5;
        double p = 0.0;
        if (style == Style::Display) {
            p = σ13;
        }
        else if (style == get_cramped_style(style)) {
            p = σ15;
        }
        else {
            p = σ14;
        }
        u = std::max({
            u,
            p,
            x.depth + std::abs(σ5) / 4.0
        });
        if (!subscript) {
            vertical_shift_box(x, u);
            atom.box = combine_boxes_horizontally({nucleus->box, x});
        }
        else {
            auto& y = subscript->box;
            horizontal_shift_box(y, scriptspace);
            y.width += scriptspace*2.5;
            v = std::max(v, σ17);
            if ((u - x.depth) - (y.height - v) < 4*θ) {
                v = 4*θ - u + x.depth + y.height;
                auto ψ = std::abs(σ5)*4/5 - (u - x.depth);
                if (ψ > 0) {
                    u += ψ;
                    v -= ψ;
                }
            }
            auto δ = 0.0; // Italic correction
            horizontal_shift_box(x, δ);
            auto distance = (x.height + u + y.depth + v)
                - (x.height + x.depth + y.height + y.depth);
            auto kern = Box(0, 0, distance, {});
            auto script_box = combine_boxes_vertically({x, kern, y}, 0);
            vertical_shift_box(script_box, (x.height + u) - script_box.height);
            atom.box = combine_boxes_horizontally({nucleus->box, script_box});
        }
    }
}

Box Processor::do_combine(Style style)
{
    auto scaling = get_style_scaling(style);
    const auto quad = get_font_quad(M_font);
    auto result_boxes = std::vector<Box>();
    auto last_type = AtomType(-1);
    for (int i = 0; i < ssize(M_rendered_list); ++i) {
        auto& noad = M_rendered_list[i];
        if (auto atom = get_if<Atom>(&noad.value)) {
            if (last_type != AtomType(-1)) {
                auto spacing = get_atom_spacing(
                    last_type,
                    atom->type
                );
                if (!spacing.second and is_script_styles(style)) {
                    spacing.first = 1;
                }
                result_boxes.push_back(
                    Box(scaling*quad*spacing.first/18.0, 0, 0, {}));
            }
            result_boxes.push_back(atom->box);
            last_type = atom->type;
        }
        else if (auto command = std::get_if<CommandNoad>(&noad.value)) {
            check_style_change(*command, style, scaling);
        }
    }
    return combine_boxes_horizontally(result_boxes);
}

Box gex::render_math_list(MathList list, Style style)
{
    using enum AtomType;
    auto processor = Processor(list);
    processor.do_preprocessing();
    processor.do_rendering(style);
    return processor.do_combine(style);
}
