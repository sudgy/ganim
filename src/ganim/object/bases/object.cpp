#include "object.hpp"

#include <iostream>

#include "ganim/ga/conversions.hpp"
#include "ganim/rate_functions.hpp"

#include "ganim/object/shape.hpp"

using namespace ganim;
using namespace pga3;

Object& Object::set_visible(bool visible)
{
    M_visible = visible;
    return *this;
}

Object& Object::set_color(Color color)
{
    M_color.back() = color;
    return *this;
}

Object& Object::push_color()
{
    M_color.push_back(M_color.back());
    return *this;
}

Object& Object::push_color(Color color)
{
    M_color.push_back(color);
    return *this;
}

Object& Object::pop_color()
{
    if (M_color.size() > 1) M_color.pop_back();
    return *this;
}

Object& Object::set_opacity(double opacity)
{
    M_opacity = opacity;
    return *this;
}

Object& Object::scale(double amount)
{
    return scale(amount, (~get_rotor()*e123*get_rotor()).grade_project<3>());
}

Object& Object::scale(double amount, const pga3::Trivec& about_point)
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

ObjectPtr<Object> Object::polymorphic_copy() const
{
    return ObjectPtr<Object>::from_new(polymorphic_copy_impl());
}

Object* Object::polymorphic_copy_impl() const
{
    class FakeObject : public Object {
        public:
            FakeObject(const Object& obj, Box true_box, Box logical_box)
                : Object(obj),
                  M_true_box(true_box),
                  M_logical_box(logical_box) {}
            Box M_true_box;
            Box M_logical_box;
            virtual Box get_original_true_bounding_box() const override
                {return M_true_box;}
            virtual Box get_original_logical_bounding_box() const override
                {return M_logical_box;}
            virtual void draw(const Camera&) override {}
            virtual void draw_outline(const Camera&) override {}
            virtual void set_outline(const Color&, double, bool) override {}
            virtual void invalidate_outline() override {}
            virtual Color get_outline_color() const override {return Color();}
            virtual double get_outline_thickness() const override {return 0.0;}
    };
    return new FakeObject(
        *this,
        get_original_true_bounding_box(),
        get_original_logical_bounding_box()
    );
}

void Object::interpolate(
    const Animatable& start,
    const Animatable& end,
    double t
)
{
    Transformable::interpolate(start, end, t);
    auto start2 = dynamic_cast<const Object*>(&start);
    auto end2 = dynamic_cast<const Object*>(&end);
    if (!start2 or !end2) return;
    auto interp = [&](std::uint8_t v1, std::uint8_t v2) {
        auto diff = static_cast<int>(v2) - static_cast<int>(v1);
        return static_cast<std::uint8_t>(static_cast<int>(v1) + diff*t);
    };
    if (start2->M_color.size() != end2->M_color.size()) {
        throw std::runtime_error("Calling push_color and pop_color is not "
                "supported for generic animations.  Use the dedicated "
                "animations instead.");
    }
    auto new_color = Color();
    new_color.r = interp(start2->M_color.back().r, end2->M_color.back().r);
    new_color.g = interp(start2->M_color.back().g, end2->M_color.back().g);
    new_color.b = interp(start2->M_color.back().b, end2->M_color.back().b);
    new_color.a = interp(start2->M_color.back().a, end2->M_color.back().a);
    set_color(new_color);
    set_opacity(start2->M_opacity + (end2->M_opacity - start2->M_opacity) * t);
    auto current_scale
        = start2->M_scale + (end2->M_scale - start2->M_scale) * t;
    if (M_scale == 0.0) {
        reset_scale();
        scale(current_scale);
    }
    else {
        scale(current_scale / M_scale);
    }
    set_draw_fraction(start2->M_draw_fraction
            + (end2->M_draw_fraction - start2->M_draw_fraction) * t);
    set_depth_z(start2->M_depth_z + (end2->M_depth_z - start2->M_depth_z) * t);
}

Box Object::get_true_bounding_box() const
{
    return transform_box(
        get_original_true_bounding_box(),
        get_rotor(),
        get_scale()
    );
}

Box Object::get_logical_bounding_box() const
{
    return transform_box(
        get_original_logical_bounding_box(),
        get_rotor(),
        get_scale()
    );
}

pga2::Bivec Object::get_center() const
{
    return get_logical_bounding_box().get_center();
}

pga3::Trivec Object::get_center_3d() const
{
    return get_logical_bounding_box().get_center_3d();
}

pga2::Bivec Object::get_left() const
{
    return get_logical_bounding_box().get_left();
}

pga2::Bivec Object::get_right() const
{
    return get_logical_bounding_box().get_right();
}

pga2::Bivec Object::get_up() const
{
    return get_logical_bounding_box().get_up();
}

pga2::Bivec Object::get_down() const
{
    return get_logical_bounding_box().get_down();
}

pga2::Bivec Object::get_upper_left() const
{
    return get_logical_bounding_box().get_upper_left();
}

pga2::Bivec Object::get_upper_right() const
{
    return get_logical_bounding_box().get_upper_right();
}

pga2::Bivec Object::get_lower_left() const
{
    return get_logical_bounding_box().get_lower_left();
}

pga2::Bivec Object::get_lower_right() const
{
    return get_logical_bounding_box().get_lower_right();
}

void Object::set_x(double x)
{
    shift((x - get_x())*e1);
}

void Object::set_y(double y)
{
    shift((y - get_y())*e2);
}

void Object::set_z(double z)
{
    shift((z - get_z())*e3);
}

double Object::get_x() const
{
    return get_logical_bounding_box()
        .get_center_3d().undual().blade_project<e1>();
}

double Object::get_y() const
{
    return get_logical_bounding_box()
        .get_center_3d().undual().blade_project<e2>();
}

double Object::get_z() const
{
    return get_logical_bounding_box()
        .get_center_3d().undual().blade_project<e3>();
}

Object& Object::next_to(
    const pga3::Trivec& point,
    const pga3::Trivec& direction,
    double buff
)
{
    auto npoint = point;
    npoint /= npoint.blade_project<e123>();

    auto ideal_direction = direction;
    auto scale = ideal_direction.blade_project<e123>();
    if (scale != 0) {
        ideal_direction /= scale;
        ideal_direction -= e123;
    }

    ideal_direction /= ideal_direction.undual().norm();
    ideal_direction *= buff;
    auto point2 = npoint + ideal_direction;
    auto point3 = get_logical_bounding_box().get_outside_point_3d(-direction);
    // Using undual makes this use the "cheat" conversion
    shift((point2 - point3).undual());

    return *this;
}

Object& Object::next_to(
    const Object& object,
    const pga3::Trivec& direction,
    double buff
)
{
    const auto other_box = object.get_logical_bounding_box();
    return next_to(other_box.get_outside_point_3d(direction), direction, buff);
}

Object& Object::align_to(
    const pga3::Trivec& point,
    const pga3::Trivec& direction
)
{
    auto ideal_direction = direction;
    auto scale = ideal_direction.blade_project<e123>();
    if (scale != 0) {
        ideal_direction /= scale;
        ideal_direction -= e123;
    }

    auto plane = point | (point & ideal_direction);
    auto outside = get_logical_bounding_box().get_outside_point_3d(direction);
    auto to_point = (outside | plane) ^ plane;
    to_point /= to_point.blade_project<e123>();
    // Using undual makes this use the "cheat" conversion
    shift((to_point - outside).undual());

    return *this;
}

Object& Object::align_to(
    const Object& object,
    const pga3::Trivec& direction
)
{
    const auto other_box = object.get_logical_bounding_box();
    return align_to(other_box.get_outside_point(direction), direction);
}

Object& Object::to_edge(
    const Camera& camera,
    const pga3::Trivec& direction,
    double buff
)
{
    using namespace vga2;
    const auto cx = camera.get_starting_width() / 2;
    const auto cy = camera.get_starting_height() / 2;
    const auto box = get_logical_bounding_box();
    const auto box_size = pga2_to_vga2(
            box.get_upper_right() - box.get_lower_left());
    const auto bx = box_size.blade_project<e1>() / 2;
    const auto by = box_size.blade_project<e2>() / 2;
    const auto sx = cx - bx - buff;
    const auto sy = cy - by - buff;
    auto d = vga3_to_vga2(pga3_to_vga3(direction));
    d /= std::max(
        std::abs(d.blade_project<e1>()),
        std::abs(d.blade_project<e2>())
    );
    const auto dx = d.blade_project<e1>();
    const auto dy = d.blade_project<e2>();
    const auto x = sx * dx;
    const auto y = sy * dy;
    shift(x*e1 + y*e2 - pga2_to_vga2(box.get_center()));
    return *this;
}

ObjectPtr<Object> Object::get_bounding_box_object(Color color) const
{
    auto box = get_logical_bounding_box();
    auto p1 = box.get_lower_left().undual();
    auto p2 = box.get_upper_left().undual();
    auto p3 = box.get_lower_right().undual();
    auto p4 = box.get_upper_right().undual();
    using namespace pga2;
    std::cout << p1.blade_project<e1>() << " " << p1.blade_project<e2>() << "\n";
    std::cout << p2.blade_project<e1>() << " " << p2.blade_project<e2>() << "\n";
    std::cout << p3.blade_project<e1>() << " " << p3.blade_project<e2>() << "\n";
    std::cout << p4.blade_project<e1>() << " " << p4.blade_project<e2>() << "\n\n";
    auto result = make_shape(
        std::vector<Shape::Vertex>{
            {float(p1.blade_project<e1>()), float(p1.blade_project<e2>())},
            {float(p2.blade_project<e1>()), float(p2.blade_project<e2>())},
            {float(p3.blade_project<e1>()), float(p3.blade_project<e2>())},
            {float(p4.blade_project<e1>()), float(p4.blade_project<e2>())},
        },
        std::vector<unsigned>{0, 1, 2, 2, 1, 3}
    );
    color.a = 127;
    result->set_color(color);
    return result;
}
