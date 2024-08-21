#include "object.hpp"

#include <iostream>

#include "ganim/ga/conversions.hpp"
#include "ganim/rate_functions.hpp"

using namespace ganim;
using namespace pga3;

Object& Object::set_visible(bool visible)
{
    M_visible = visible;
    return *this;
}

Object& Object::set_color(Color color)
{
    color.a = M_color.a;
    return set_color_with_alpha(color);
}

Object& Object::set_color_with_alpha(Color color)
{
    M_color = color;
    return *this;
}

Object& Object::set_opacity(double opacity)
{
    auto color = M_color;
    opacity = std::clamp(opacity, 0.0, 1.0);
    color.a = static_cast<std::uint8_t>(opacity * 255.0);
    return set_color_with_alpha(color);
}

Object& Object::scale(double amount)
{
    return scale((~get_rotor()*e123*get_rotor()).grade_project<3>(), amount);
}

Object& Object::scale(const pga3::Trivector& about_point, double amount)
{
    auto new_about_point = about_point;
    new_about_point /= new_about_point.blade_project<e123>();
    new_about_point -= new_about_point.blade_project<e123>()*e123;
    auto center = (~get_rotor()*e123*get_rotor()).grade_project<3>();
    center -= new_about_point;
    auto new_center = center;
    new_center -= new_center.blade_project<e123>()*e123;
    new_center *= amount;
    new_center += e123;
    shift(new_center - center + e123);
    M_scale *= amount;
    return *this;
}

double Object::get_scale() const
{
    return M_scale;
}

std::unique_ptr<Object> Object::anim_copy() const
{
    class FakeObject : public Object {
        public:
            FakeObject(const Object& obj, Box true_box, Box logical_box)
                : Object(obj),
                  M_true_box(true_box),
                  M_logical_box(logical_box) {}
            Box M_true_box;
            Box M_logical_box;
            virtual Box get_true_bounding_box() const override
                {return M_true_box;}
            virtual Box get_logical_bounding_box() const override
                {return M_logical_box;}
            virtual void draw(const Camera&) override {}
            virtual void draw_outline(const Camera&) override {}
            virtual void set_outline(const Color&, double) override {}
            virtual void invalidate_outline() override {}
            virtual Color get_outline_color() const override {return Color();}
            virtual double get_outline_thickness() const override {return 0.0;}
    };
    return std::make_unique<FakeObject>(
        *this,
        get_true_bounding_box(),
        get_logical_bounding_box()
    );
}

void Object::interpolate(const Object& start, const Object& end, double t)
{
    Transformable::interpolate(start, end, t);
    auto interp = [&](std::uint8_t v1, std::uint8_t v2) {
        auto diff = static_cast<int>(v2) - static_cast<int>(v1);
        return static_cast<std::uint8_t>(static_cast<int>(v1) + diff*t);
    };
    auto new_color = Color();
    new_color.r = interp(start.M_color.r, end.M_color.r);
    new_color.g = interp(start.M_color.g, end.M_color.g);
    new_color.b = interp(start.M_color.b, end.M_color.b);
    new_color.a = interp(start.M_color.a, end.M_color.a);
    set_color_with_alpha(new_color);
    auto current_scale
        = start.M_scale + (end.M_scale - start.M_scale) * t;
    scale(current_scale / M_scale);
    set_draw_fraction(start.M_draw_fraction
            + (end.M_draw_fraction - start.M_draw_fraction) * t);
}
