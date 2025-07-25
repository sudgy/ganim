#include "group.hpp"

#include <format>
#include <ranges>
#include <numeric>

#include "group.hpp"

using namespace ganim;

Group::Group(const Group& other)
:   Object(other),
    M_ratio(other.M_ratio),
    M_outline_thickness(other.M_outline_thickness),
    M_outline_color(other.M_outline_color),
    M_propogate(other.M_propogate),
    M_draw_together(other.M_draw_together)
{
    for (auto& obj : other.M_subobjects) {
        add(obj->polymorphic_copy());
    }
}

Group::Group(const Group& other, std::nullptr_t)
:   Object(other),
    M_ratio(other.M_ratio),
    M_outline_thickness(other.M_outline_thickness),
    M_outline_color(other.M_outline_color),
    M_propogate(other.M_propogate),
    M_draw_together(other.M_draw_together) {}

void Group::copy_members(const Group& other)
{
    M_ratio = other.M_ratio;
    M_outline_thickness = other.M_outline_thickness;
    M_outline_color = other.M_outline_color;
    M_propogate = other.M_propogate;
    M_draw_together = other.M_draw_together;
}

Group& Group::operator=(const Group& other)
{
    Object::operator=(other);
    copy_members(other);
    clear();
    for (auto& obj : other.M_subobjects) {
        add(obj->polymorphic_copy());
    }
    return *this;
}

void Group::add(ObjectPtr<Object> object)
{
    M_new_subobjects.push_back(object);
    M_subobjects.push_back(std::move(object));
}

void Group::remove(Object& object)
{
    for (auto it = M_subobjects.begin(); it != M_subobjects.end(); ++it) {
        if (&**it == &object) {
            M_subobjects.erase(it);
            break;
        }
    }
    for (auto it = M_new_subobjects.begin(); it != M_new_subobjects.end();++it){
        if (&**it == &object) {
            M_new_subobjects.erase(it);
            break;
        }
    }
}

ObjectPtr<Group> Group::polymorphic_copy() const
{
    return ObjectPtr<Group>::from_new(polymorphic_copy_impl());
}

Group* Group::polymorphic_copy_impl() const
{
    return new Group(*this);
}

void Group::interpolate(const Animatable& start, const Animatable& end, double t)
{
    auto start2 = dynamic_cast<const Group*>(&start);
    auto end2 = dynamic_cast<const Group*>(&end);
    if (!start2) throw std::invalid_argument(
        "Interpolating a group between non-groups is not supported.");
    if (!end2) throw std::invalid_argument(
        "Interpolating a group between non-groups is not supported.");
    if (size() != start2->size()) {
        throw std::invalid_argument(std::format(
            "When interpolating a group of size {}, the size of the starting "
            "group is {}, when the two sizes should be equal.",
            size(),
            start2->size()
        ));
    }
    if (size() != end2->size()) {
        throw std::invalid_argument(std::format(
            "When interpolating a group of size {}, the size of the ending "
            "group is {}, when the two sizes should be equal.",
            size(),
            end2->size()
        ));
    }
    for (auto i = 0; i < size(); ++i) {
        M_subobjects[i]->interpolate(*(*start2)[i], *(*end2)[i], t);
    }
    M_propogate = false;
    Object::interpolate(start, end, t);
    M_propogate = true;
}

void Group::draw(const Camera& camera)
{
    if (!M_draw_together) return;
    for (auto drawable : M_subobjects) {
        if (drawable->is_visible()) {
            drawable->draw(camera);
        }
    }
}

bool Group::is_visible() const
{
    for (auto drawable : M_subobjects) {
        if (drawable->is_visible()) return true;
    }
    return false;
}

void Group::draw_outline(const Camera& camera)
{
    if (!M_draw_together) return;
    for (auto drawable : M_subobjects) {
        if (drawable->is_visible()) {
            drawable->draw_outline(camera);
        }
    }
}

void Group::set_outline(const Color& color, double thickness, bool shift_depth)
{
    M_outline_color = color;
    M_outline_thickness = thickness;
    for (auto drawable : M_subobjects) {
        drawable->set_outline(color, thickness, shift_depth);
    }
}

void Group::invalidate_outline()
{
    for (auto drawable : M_subobjects) {
        drawable->invalidate_outline();
    }
}

Color Group::get_outline_color() const
{
    return M_outline_color;
}

double Group::get_outline_thickness() const
{
    return M_outline_thickness;
}

void Group::set_peeling_depth_buffer(gl::Texture* texture)
{
    Object::set_peeling_depth_buffer(texture);
    for (auto obj : M_subobjects) {
        obj->set_peeling_depth_buffer(texture);
    }
}

void Group::set_fps(int fps)
{
    Animatable::set_fps(fps);
    for (auto obj : M_subobjects) {
        obj->set_fps(fps);
    }
}

Group& Group::apply_rotor(const pga3::Even& rotor)
{
    Transformable::apply_rotor(rotor);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->apply_rotor(rotor);
        }
    }
    return *this;
}

Group& Group::set_color(Color color)
{
    Object::set_color(color);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_color(color);
        }
    }
    return *this;
}

Group& Group::push_color()
{
    Object::push_color();
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->push_color();
        }
    }
    return *this;
}

Group& Group::push_color(Color color)
{
    Object::push_color(color);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->push_color(color);
        }
    }
    return *this;
}

Group& Group::pop_color()
{
    Object::pop_color();
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->pop_color();
        }
    }
    return *this;
}

Group& Group::set_opacity(double opacity)
{
    Object::set_opacity(opacity);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_opacity(opacity);
        }
    }
    return *this;
}

Group& Group::scale(double amount, const pga3::Trivec& about_point)
{
    if (M_propogate) {
        M_propogate = false;
        Object::scale(amount, about_point);
        M_propogate = true;
        for (auto obj : M_subobjects) {
            obj->scale(amount, about_point);
        }
    }
    else Object::scale(amount, about_point);
    return *this;
}

Group& Group::set_visible(bool visible)
{
    Object::set_visible(visible);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_visible(visible);
        }
    }
    return *this;
}

void Group::set_draw_fraction(double value)
{
    Object::set_draw_fraction(value);
    const auto s = size();
    if (s == 0) return;
    const auto total_t = 1 + M_ratio * (s - 1);
    for (auto i = 0; i < s; ++i) {
        auto this_value = value * total_t - i * M_ratio;
        M_subobjects[i]->set_draw_fraction(std::clamp(this_value, 0.0, 1.0));
    }
}

Group& Group::set_depth_z(double depth_z)
{
    Object::set_depth_z(depth_z);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_depth_z(depth_z);
        }
    }
    return *this;
}

void Group::set_creating(bool creating)
{
    Object::set_creating(creating);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_creating(creating);
        }
    }
}

void Group::set_noise_creating(double noise_creating)
{
    Object::set_noise_creating(noise_creating);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_noise_creating(noise_creating);
        }
    }
}

void Group::set_fixed_in_frame(bool fixed_in_frame)
{
    Object::set_fixed_in_frame(fixed_in_frame);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_fixed_in_frame(fixed_in_frame);
        }
    }
}

void Group::set_fixed_orientation(bool fixed_orientation)
{
    Object::set_fixed_orientation(fixed_orientation);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_fixed_orientation(fixed_orientation);
        }
    }
}

Box Group::get_true_bounding_box() const
{
    return get_original_true_bounding_box();
}

Box Group::get_logical_bounding_box() const
{
    return get_original_logical_bounding_box();
}

Box Group::get_original_true_bounding_box() const
{
    if (size() == 0) return Box();
    auto boxes = M_subobjects
        | std::views::transform(&Object::get_true_bounding_box);
    return std::reduce(
        boxes.begin()+1,
        boxes.end(),
        *boxes.begin(),
        merge_boxes
    );
}

Box Group::get_original_logical_bounding_box() const
{
    if (size() == 0) return Box();
    auto boxes = M_subobjects
        | std::views::transform(&Object::get_logical_bounding_box);
    return std::reduce(
        boxes.begin()+1,
        boxes.end(),
        *boxes.begin(),
        merge_boxes
    );
}

void Group::set_animating(bool animating)
{
    Object::set_animating(animating);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_animating(animating);
        }
    }
}

bool Group::is_animating() const
{
    if (Object::is_animating()) return true;
    for (auto obj : M_subobjects) {
        if (obj->is_animating()) return true;
    }
    return false;
}

void Group::set_draw_subobject_ratio(double ratio)
{
    if (ratio < 0 or ratio > 1) throw std::invalid_argument(
            "The draw subobject ratio must be between zero and 1.");
    M_ratio = ratio;
}

double Group::get_draw_subobject_ratio() const
{
    return M_ratio;
}

ObjectPtr<Group> Group::range(int i1, int i2)
{
    auto result = make_group();
    if (i1 < 0) i1 = size() + i1;
    if (i2 < 0) i2 = size() + i2;
    if (i1 < 0) {
        throw std::out_of_range(
                "Starting index too low when finding a range of a group");
    }
    if (i2 > size()) {
        throw std::out_of_range(
                "Ending index too high when finding a range of a group");
    }
    for (int i = i1; i < i2; ++i) {
        result->add(M_subobjects[i]);
    }
    result->set_draw_subobject_ratio(M_ratio);
    if (M_draw_together) result->draw_together();
    if (is_fixed_in_frame()) result->set_fixed_in_frame(true);
    if (is_fixed_orientation()) result->set_fixed_orientation(true);
    return result;
}

ObjectPtr<Group> Group::range(int i)
{
    auto result = make_group();
    if (i < 0) i = size() + i;
    if (i < 0) {
        throw std::out_of_range(
                "Starting index too low when finding a range of a group");
    }
    if (i >= size()) {
        throw std::out_of_range(
                "Starting index too high when finding a range of a group");
    }
    for (; i < size(); ++i) {
        result->add(M_subobjects[i]);
    }
    result->set_draw_subobject_ratio(M_ratio);
    if (M_draw_together) result->draw_together();
    if (is_fixed_in_frame()) result->set_fixed_in_frame(true);
    if (is_fixed_orientation()) result->set_fixed_orientation(true);
    return result;
}

Group& Group::align_by_subobject(
    int index,
    const pga3::Trivec& point,
    const pga3::Trivec& direction
)
{
    auto& subobject = *M_subobjects[index];
    auto orig_pos = subobject.get_center();
    subobject.align_to(point, direction);
    auto new_pos = subobject.get_center();
    auto dir = (new_pos - orig_pos).undual();
    subobject.shift(-dir);
    shift(dir);
    return *this;
}

Group& Group::align_by_subobject(
    int index,
    const Object& object,
    const pga3::Trivec& direction
)
{
    const auto other_box = object.get_logical_bounding_box();
    return align_by_subobject(
        index,
        other_box.get_outside_point(direction),
        direction
    );
}

Group& Group::arrange_down(ArrangeArgs args)
{
    if (size() == 0) return *this;
    auto buff = args.buff;
    auto align = args.align;
    auto* current = M_subobjects[0].get();
    for (int i = 1; i < size(); ++i) {
        auto* next = M_subobjects[i].get();
        next->next_to(*current, -vga2::e2, buff);
        current = next;
    }
    if (align != 0) {
        for (auto& obj : M_subobjects) {
            obj->align_to(*this, align);
        }
    }
    auto fake_group = Group();
    for (auto& obj : M_subobjects) {
        fake_group.add(obj);
    }
    auto fake_center = fake_group.get_center();
    auto this_center = pga3_to_pga2(get_origin());
    fake_group.shift(this_center - fake_center + pga2::e12);
    return *this;
}

Group& Group::arrange_right(ArrangeArgs args)
{
    if (size() == 0) return *this;
    auto buff = args.buff;
    auto align = args.align;
    auto* current = M_subobjects[0].get();
    for (int i = 1; i < size(); ++i) {
        auto* next = M_subobjects[i].get();
        next->next_to(*current, vga2::e1, buff);
        current = next;
    }
    if (align != 0) {
        for (auto& obj : M_subobjects) {
            obj->align_to(*this, align);
        }
    }
    auto fake_group = Group();
    for (auto& obj : M_subobjects) {
        fake_group.add(obj);
    }
    auto fake_center = fake_group.get_center();
    auto this_center = pga3_to_pga2(get_origin());
    fake_group.shift(this_center - fake_center + pga2::e12);
    return *this;
}

Group& Group::arrange_in_grid(
    int columns,
    ArrangeArgs hor_args,
    ArrangeArgs ver_args
)
{
    const auto size = this->size();
    if (size == 0) return *this;
    auto row_groups = std::vector<Group>();
    auto col_groups = std::vector<Group>();
    const auto rows = (size - 1) / columns + 1;
    row_groups.resize(rows);
    col_groups.resize(columns);
    for (int i = 0; i < size; ++i) {
        row_groups[i / columns].add(M_subobjects[i]);
        col_groups[i % columns].add(M_subobjects[i]);
    }

    auto heights = std::vector<double>();
    auto widths = std::vector<double>();
    heights.resize(rows);
    widths.resize(columns);
    for (int i = 0; i < rows; ++i) {
        for (auto obj : row_groups[i]) {
            heights[i] = std::max(
                heights[i],
                obj->get_logical_bounding_box().get_height()
            );
        }
    }
    for (int i = 0; i < columns; ++i) {
        for (auto obj : col_groups[i]) {
            widths[i] = std::max(
                widths[i],
                obj->get_logical_bounding_box().get_width()
            );
        }
    }

    auto xs = std::vector<double>();
    auto ys = std::vector<double>();
    xs.resize(columns);
    ys.resize(rows);
    for (int i = 1; i < columns; ++i) {
        xs[i] = xs[i-1] + widths[i-1] / 2 + widths[i] / 2 + hor_args.buff;
    }
    for (int i = 1; i < rows; ++i) {
        ys[i] = ys[i-1] - heights[i-1] / 2 - heights[i] / 2 - ver_args.buff;
    }
    const auto total_width = xs.back() + widths.front()/2 + widths.back()/2;
    const auto total_height = -ys.back() + heights.front()/2 + heights.back()/2;
    const auto o = get_origin().undual();
    const auto x_plus
        = -total_width / 2 + o.blade_project<pga3::e1>() + widths[0] / 2;
    const auto y_plus
        = total_height / 2 + o.blade_project<pga3::e2>() - heights[0] / 2;
    for (auto& x : xs) x += x_plus;
    for (auto& y : ys) y += y_plus;

    for (int i = 0; i < size; ++i) {
        auto final_pos = xs[i % columns] * vga2::e1 +
                         ys[i / columns] * vga2::e2;
        auto current_pos = pga2_to_vga2(M_subobjects[i]->get_center());
        M_subobjects[i]->shift(final_pos - current_pos);
    }

    if (ver_args.align != 0) {
        for (int i = 0; i < rows; ++i) {
            for (auto obj : row_groups[i]) {
                obj->align_to(row_groups[i], ver_args.align);
            }
        }
    }
    if (hor_args.align != 0) {
        for (int i = 0; i < columns; ++i) {
            for (auto obj : col_groups[i]) {
                obj->align_to(col_groups[i], hor_args.align);
            }
        }
    }

    return *this;
}
