#include "gex.hpp"

#include "gex/gex.hpp"

using namespace ganim;

Gex::Gex(const std::vector<std::string_view>& strings)
{
    draw_together();
    set_draw_subobject_ratio(0.2);
    M_font = &get_font("fonts/NewCM10-Regular.otf", 128);
    create(strings);
}

std::vector<PositionedGlyph> Gex::get_glyphs(
        const std::vector<std::string_view>& strings)
{
    return gex_render(strings);
}

ObjectPtr<Gex> Gex::polymorphic_copy() const
{
    return ObjectPtr<Gex>::from_new(polymorphic_copy_impl());
}

Gex* Gex::polymorphic_copy_impl() const
{
    return new Gex(*this);
}
