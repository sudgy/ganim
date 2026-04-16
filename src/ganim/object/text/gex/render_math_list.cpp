#include "render_math_list.hpp"

#include <algorithm>

#include "ganim/fmap.hpp"

#include "math_letter_map.hpp"
#include "accent_offsets.hpp"
#include "split.hpp"
#include "section_render.hpp"
#include "section_combine.hpp"

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
                M_font(&get_font("fonts/NewCMMath-Regular.otf")) {}
            void do_preprocessing();
            void do_rendering(Style style);
            Box do_combine(Style style);

        private:
            void render_atom(Atom& atom, Style style);
            void render_atom_symbol(
                Atom& atom,
                AtomSymbol& symbol,
                Style style
            );
            void render_atom_list(Atom& atom, AtomList& list, Style style);
            void render_atom_script(Atom& atom, AtomScript& script,Style style);
            void render_atom_accent(Atom& atom, AtomAccent& accent,Style style);
            void render_atom_radical(
                Atom& atom, AtomRadical& radical, Style style);
            void render_atom_tokens(Atom& atom, AtomTokens& tokens,Style style);
            Noad render_fraction(FractionNoad& fraction, Style style);
            Noad render_boundary(BoundaryNoad& boundary, Style style);
            Box make_delimiter(
                std::uint32_t codepoint, int group, double height, Style style);

            MathList& M_list;
            MathList M_rendered_list;
            Font* M_font = nullptr;
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
        else if (holds_alternative<FractionNoad>(noad.value)) {
            last_type = AtomType::Ord;
        }
        else if (holds_alternative<BoundaryNoad>(noad.value)) {
            last_type = AtomType::Ord;
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
        auto glyphs = shape_text_manual_groups(*M_font, text);
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
            std::visit(overloaded{
                [&](Atom& atom)
                {
                    render_atom(atom, style);
                    M_rendered_list.push_back(Noad(atom));
                },
                [&](CommandNoad& command)
                {
                    check_style_change(command, style, scaling);
                    M_rendered_list.push_back(noad);
                },
                [&](FractionNoad& fraction)
                {
                    M_rendered_list.push_back(render_fraction(fraction, style));
                },
                [&](BoundaryNoad& boundary)
                {
                    M_rendered_list.push_back(render_boundary(boundary, style));
                },
                [&](GlueNoad&)
                {
                    M_rendered_list.push_back(noad);
                }
            }, noad.value);
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
        [&](AtomScript& script)
            {render_atom_script(atom, script, style);},
        [&](AtomAccent& accent)
            {render_atom_accent(atom, accent, style);},
        [&](AtomRadical& radical)
            {render_atom_radical(atom, radical, style);},
        [&](AtomTokens& tokens)
            {render_atom_tokens(atom, tokens, style);},
        [&](auto&) {}
    }, atom.value);
}

void Processor::render_atom_symbol(
    Atom& atom,
    AtomSymbol& symbol,
    Style style
)
{
    auto it = math_letter_map.find(symbol.codepoint);
    auto codepoint = it == math_letter_map.end()
        ? symbol.codepoint
        : it->second;
    auto scaling = get_style_scaling(style);
    auto glyphs = shape_text_manual_groups(
        *M_font,
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
    AtomScript& script,
    Style style
)
{
    Atom* nucleus = script.nucleus.get();
    Atom* subscript = script.subscript.get();
    Atom* superscript = script.superscript.get();
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

    const double scriptspace = get_font_em(*M_font) / 20.0;
    const double σ5 = get_font_x_height(*M_font);
    const double σ13 = get_font_sup1(*M_font);
    const double σ14 = get_font_sup2(*M_font);
    const double σ15 = get_font_sup3(*M_font);
    const double σ16 = get_font_sub1(*M_font);
    const double σ17 = get_font_sub2(*M_font);
    const double σ18 = get_font_sup_drop(*M_font);
    const double σ19 = get_font_sub_drop(*M_font);
    const auto θ = get_font_default_rule_thickness(*M_font);

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
        x.width += scriptspace;
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
        x.width += scriptspace;
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
            y.width += scriptspace;
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
            if (auto symbol = get_if<AtomSymbol>(&nucleus->value)) {
                switch (symbol->codepoint) {
                case U'f':
                    δ = -0.2;
                    break;
                }
            }
            horizontal_shift_box(y, δ);
            auto distance = (x.height + u + y.depth + v)
                - (x.height + x.depth + y.height + y.depth);
            auto kern = Box(0, 0, distance, {});
            auto script_box = combine_boxes_vertically({x, kern, y}, 0);
            vertical_shift_box(script_box, (x.height + u) - script_box.height);
            atom.box = combine_boxes_horizontally({nucleus->box, script_box});
        }
    }
}

void Processor::render_atom_accent(Atom& atom, AtomAccent& accent, Style style)
{
    auto codepoint = 0;
    Atom* nucleus = accent.nucleus.get();
    while (true) {
        if (auto list = get_if<AtomList>(&nucleus->value)) {
            if (list->list.size() == 1) {
                if (auto new_nucleus = get_if<Atom>(&list->list[0].value)) {
                    nucleus = new_nucleus;
                    continue;
                }
            }
        }
        break;
    }
    if (auto symbol = get_if<AtomSymbol>(&nucleus->value)) {
        auto it = math_letter_map.find(symbol->codepoint);
        codepoint = it == math_letter_map.end()
            ? symbol->codepoint
            : it->second;
    }

    render_atom(*accent.accent, style);
    scale_box(accent.accent->box, accent.scale);
    render_atom(*accent.nucleus, get_cramped_style(style));
    auto& a = accent.accent->box;
    auto& n = accent.nucleus->box;
    auto height = std::max(n.height, get_font_x_height(*M_font)) + 0.05;
    for (auto& glyph : a.glyphs) {
        glyph.y_pos += height;
        glyph.draw_y = glyph.y_pos + glyph.height;
        glyph.y_min += height;
        glyph.y_max += height;
    }
    if (n.width > a.width) {
        auto x_dif = (n.width - a.width) / 2.0;
        for (auto& glyph : a.glyphs) {
            glyph.x_pos += x_dif;
            glyph.draw_x += x_dif;
        }
    }
    else {
        auto x_dif = (a.width - n.width) / 2.0;
        for (auto& glyph : n.glyphs) {
            glyph.x_pos += x_dif;
            glyph.draw_x += x_dif;
        }
    }
    const double offset = get_accent_offset(codepoint);
    if (offset > 0) {
        for (auto& glyph : a.glyphs) {
            glyph.x_pos += offset;
            glyph.draw_x += offset;
        }
    }
    atom.box.width = std::max(a.width, n.width);
    atom.box.height = height + a.height;
    atom.box.depth = n.depth;
    atom.box.glyphs.append_range(a.glyphs);
    atom.box.glyphs.append_range(n.glyphs);
}

void Processor::render_atom_radical(
    Atom& atom,
    AtomRadical& radical,
    Style style
)
{
    render_atom(*radical.nucleus, get_cramped_style(style));
    auto& x = radical.nucleus->box;
    auto θ = get_font_default_rule_thickness(*M_font);
    auto φ = 0.0;
    if (style == Style::Display or style == Style::CrampedDisplay) {
        φ = get_font_x_height(*M_font);
    }
    else {
        φ = θ;
    }
    auto ψ = θ + std::abs(φ)/4.0;
    auto y = make_delimiter(
        radical.radical,
        radical.group,
        x.height + x.depth + ψ + θ,
        style
    );
    y.glyphs[0].y_min = 0;
    y.glyphs[0].y_max = 0;
    θ = y.height;
    if (y.depth > x.height + x.depth + ψ) {
        ψ = 0.5*(ψ + y.depth - x.height - x.depth);
    }
    auto kernθ = Box(0, 0, θ, {});
    auto rule = make_rule(x.width, θ - 0.0025, 0, radical.group);
    auto kernψ = Box(0, 0, ψ, {});
    auto vbox = combine_boxes_vertically({kernθ, rule, kernψ, x}, 3);
    vertical_shift_box(y, x.height + ψ);
    auto nkern = Box(-θ*0.6, 0, 0, {});
    atom.box = combine_boxes_horizontally({y, nkern, vbox});
}

void Processor::render_atom_tokens(
    Atom& atom,
    AtomTokens& tokens,
    Style style
)
{
    auto sections = split(tokens.list);
    auto rendered_sections = fmap(
        sections,
        [&](const auto& section) {return section_render(section, style);}
    );
    atom.box = section_combine(rendered_sections);
}

Noad Processor::render_fraction(FractionNoad& fraction, Style style)
{
    auto θ = fraction.rule_thickness;
    if (θ < 0.0) θ = get_font_default_rule_thickness(*M_font);
    auto x = render_math_list(
        fraction.numerator,
        get_numerator_style(style)
    );
    auto z = render_math_list(
        fraction.denominator,
        get_denominator_style(style)
    );
    if (x.width > z.width) {
        horizontal_shift_box(z, (x.width - z.width) / 2.0);
        z.width = x.width;
    }
    else if (z.width > x.width) {
        horizontal_shift_box(x, (z.width - x.width) / 2.0);
        x.width = z.width;
    }
    auto u = 0.0;
    auto v = 0.0;
    bool display = style == Style::Display or style == Style::CrampedDisplay;
    if (display) {
        u = get_font_num1(*M_font);
        v = get_font_denom1(*M_font);
    }
    else {
        if (θ == 0) u = get_font_num3(*M_font);
        else u = get_font_num2(*M_font);
        v = get_font_denom2(*M_font);
    }
    auto box = Box();
    if (θ == 0) {
        auto φ = get_font_default_rule_thickness(*M_font);
        if (display) φ *= 7;
        else φ *= 3;
        auto ψ = (u - x.depth) - (z.height - v);
        if (ψ < φ) {
            auto plus = (φ - ψ)/2;
            u += plus;
            v += plus;
        }
        auto distance = (x.height + u + z.depth + v)
            - (x.height + x.depth + z.height + z.depth);
        auto kern = Box(0, 0, distance, {});
        box = combine_boxes_vertically({x, kern, z}, 0);
        vertical_shift_box(box, (x.height + u) - box.height);
    }
    else {
        auto φ = get_font_default_rule_thickness(*M_font);
        if (display) φ *= 3;
        auto a = get_font_axis_height(*M_font);
        auto diff = (u - x.depth) - (a + θ/2);
        if (diff < φ) u += φ - diff;
        diff = (a - θ/2) - (z.height - v);
        if (diff < φ) v += φ - diff;
        auto kern1 = Box(0, 0, u - x.depth - a - θ/2, {});
        auto rule = make_rule(x.width, θ, 0, fraction.group);
        auto kern2 = Box(0, 0, a - θ/2 + (v - z.height), {});
        box = combine_boxes_vertically({x, kern1, rule, kern2, z}, 0);
        vertical_shift_box(box, (x.height + u) - box.height);
    }
    auto a = get_font_axis_height(*M_font);
    auto fraction_height = std::max((box.height - a), (box.depth + a)) * 2 +0.1;
    auto make_delim = [&](std::uint32_t codepoint) {
        auto box = make_delimiter(
            codepoint, fraction.group, fraction_height, style);
        auto final_height = box.height;
        auto final_depth = box.depth;
        vertical_shift_box(box, (final_depth - final_height) / 2 + a);
        return box;
    };
    auto delim1 = make_delim(fraction.left_delim);
    auto delim2 = make_delim(fraction.right_delim);
    auto final_box = combine_boxes_horizontally({delim1, box, delim2});
    return {Atom(std::move(final_box), AtomType::Ord, AtomBox())};
}

Noad Processor::render_boundary(BoundaryNoad& boundary, Style style)
{
    auto inside = render_math_list(
        boundary.inside,
        style
    );
    auto a = get_font_axis_height(*M_font);
    auto height = std::max((inside.height - a), (inside.depth + a)) * 2;
    auto make_delim = [&](std::uint32_t codepoint, int group) {
        auto box = make_delimiter(codepoint, group, height, style);
        auto final_height = box.height;
        auto final_depth = box.depth;
        vertical_shift_box(box, (final_depth - final_height) / 2 + a);
        return box;
    };
    auto delim1 = make_delim(boundary.left_delim, boundary.left_group);
    auto delim2 = make_delim(boundary.right_delim, boundary.right_group);
    auto kern = Box(delim1.width / 3.0, 0, 0, {});
    auto final_box = combine_boxes_horizontally(
        {kern, delim1, inside, delim2, kern}
    );
    return {Atom(std::move(final_box), AtomType::Ord, AtomBox())};
}

Box Processor::make_delimiter(
    std::uint32_t codepoint, int group, double height, Style style)
{
    // TODO: Add \nulldelimiterspace?
    if (codepoint == 0) return Box(0.12, 0, 0, {});
    (void)style; // Is this bad?
    auto glyphs = shape_delimiter(*M_font, codepoint, group, height);
    return box_from_glyphs(glyphs);
}

Box Processor::do_combine(Style style)
{
    auto scaling = get_style_scaling(style);
    const auto quad = get_font_quad(*M_font);
    auto result_boxes = std::vector<Box>();
    auto last_type = AtomType(-1);
    bool phantom = false;
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
            if (phantom) {
                for (auto& glyph : atom->box.glyphs) {
                    glyph.invisible = true;
                }
                phantom = false;
            }
            result_boxes.push_back(atom->box);
            last_type = atom->type;
        }
        else if (auto command = std::get_if<CommandNoad>(&noad.value)) {
            check_style_change(*command, style, scaling);
            if (command->command == "phantom") {
                phantom = true;
            }
        }
        else if (auto glue = std::get_if<GlueNoad>(&noad.value)) {
            result_boxes.push_back(Box(glue->thickness, 0, 0, {}));
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
