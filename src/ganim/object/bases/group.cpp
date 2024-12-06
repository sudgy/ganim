#include "group.hpp"

#include <format>
#include <ranges>
#include <numeric>

#include "group.hpp"

using namespace ganim;

void Group::add(Object& object)
{
    M_subobjects.push_back(&object);
}

std::unique_ptr<Group> Group::anim_copy() const
{
    struct OwningGroup : public Group {
        OwningGroup(const Group& other) : Group(other) {}
        std::vector<std::unique_ptr<Object>> M_owned_subobjects;
        virtual void draw(const Camera&) override {}
        virtual void draw_outline(const Camera&) override {}
        virtual void set_outline(const Color&, double) override {}
        virtual void invalidate_outline() override {}
        virtual Color get_outline_color() const override {return Color();}
        virtual double get_outline_thickness() const override {return 0.0;}
    };
    auto result = std::make_unique<OwningGroup>(*this);
    result->M_subobjects.clear();
    for (auto obj : M_subobjects) {
        if (auto subgroup = obj->as_group()) {
            result->M_owned_subobjects.emplace_back(subgroup->anim_copy());
        }
        else {
            result->M_owned_subobjects.emplace_back(obj->anim_copy());
        }
        result->M_subobjects.push_back(result->M_owned_subobjects.back().get());
    }
    return result;
}

void Group::interpolate(const Group& start, const Group& end, double t)
{
    if (size() != start.size()) {
        throw std::invalid_argument(std::format(
            "When interpolating a group of size {}, the size of the starting "
            "group is {}, when the two sizes should be equal.",
            size(),
            start.size()
        ));
    }
    if (size() != end.size()) {
        throw std::invalid_argument(std::format(
            "When interpolating a group of size {}, the size of the ending "
            "group is {}, when the two sizes should be equal.",
            size(),
            end.size()
        ));
    }
    for (auto i = 0; i < size(); ++i) {
        auto group1 = (*this)[i].as_group();
        auto group2 = start[i].as_group();
        auto group3 = end[i].as_group();
        if (group1) {
            if (!group2) {
                throw std::invalid_argument(std::format(
                    "When interpolating a group, the object at index {} is a "
                    "subgroup, while it is not a subgroup in the starting "
                    "group.",
                    i
                ));
            }
            if (!group3) {
                throw std::invalid_argument(std::format(
                    "When interpolating a group, the object at index {} is a "
                    "subgroup, while it is not a subgroup in the ending "
                    "group.",
                    i
                ));
            }
        }
        else {
            if (group2) {
                throw std::invalid_argument(std::format(
                    "When interpolating a group, the object at index {} is not "
                    "a subgroup, while it is a subgroup in the starting group.",
                    i
                ));
            }
            if (group3) {
                throw std::invalid_argument(std::format(
                    "When interpolating a group, the object at index {} is not "
                    "a subgroup, while it is not a subgroup in the ending ",
                    "group.",
                    i
                ));
            }
        }
    }
    for (auto i = 0; i < size(); ++i) {
        auto& obj = (*this)[i];
        if (auto group = obj.as_group()) {
            group->interpolate(*start[i].as_group(), *end[i].as_group(), t);
        }
        else {
            obj.interpolate(start[i], end[i], t);
        }
    }
    M_propogate = false;
    Object::interpolate(start, end, t);
    M_propogate = true;
}

void Group::interpolate(
    const Transformable& start,
    const Transformable& end,
    double t
)
{
    auto start_group = start.as_group();
    auto end_group = end.as_group();
    if (!start_group) throw std::invalid_argument(
        "Interpolating a group between two non-groups is not supported.");
    if (!end_group) throw std::invalid_argument(
        "Interpolating a group between two non-groups is not supported.");
    interpolate(*start_group, *end_group, t);
}

void Group::interpolate(
    const Object& start,
    const Object& end,
    double t
)
{
    auto start_group = start.as_group();
    auto end_group = end.as_group();
    if (!start_group) throw std::invalid_argument(
        "Interpolating a group between two non-groups is not supported.");
    if (!end_group) throw std::invalid_argument(
        "Interpolating a group between two non-groups is not supported.");
    interpolate(*start_group, *end_group, t);
}

void Group::draw(const Camera& camera)
{
    for (auto drawable : M_subobjects) {
        if (drawable->is_visible()) {
            if (!M_draw_together) drawable->draw_outline(camera);
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

void Group::set_outline(const Color& color, double thickness)
{
    M_outline_color = color;
    M_outline_thickness = thickness;
    for (auto drawable : M_subobjects) {
        drawable->set_outline(color, thickness);
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
    auto new_color = color;
    new_color.a = get_color().a;
    Object::set_color_with_alpha(new_color);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_color(color);
        }
    }
    return *this;
}

Group& Group::set_color_with_alpha(Color color)
{
    Object::set_color_with_alpha(color);
    if (M_propogate) {
        for (auto obj : M_subobjects) {
            obj->set_color_with_alpha(color);
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

Group& Group::scale(const pga3::Trivec& about_point, double amount)
{
    if (M_propogate) {
        M_propogate = false;
        Object::scale(about_point, amount);
        M_propogate = true;
        for (auto obj : M_subobjects) {
            obj->scale(about_point, amount);
        }
    }
    else Object::scale(about_point, amount);
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

Group Group::range(int i1, int i2)
{
    auto result = Group();
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
        result.add((*this)[i]);
    }
    result.set_draw_subobject_ratio(M_ratio);
    if (M_draw_together) result.draw_together();
    return result;
}

Group Group::range(int i)
{
    auto result = Group();
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
        result.add((*this)[i]);
    }
    result.set_draw_subobject_ratio(M_ratio);
    if (M_draw_together) result.draw_together();
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
    auto* current = M_subobjects[0];
    for (int i = 1; i < size(); ++i) {
        auto* next = M_subobjects[i];
        next->next_to(*current, -vga2::e2, buff);
        current = next;
    }
    if (align != 0) {
        for (auto* obj : M_subobjects) {
            obj->align_to(*this, align);
        }
    }
    auto fake_group = Group();
    for (auto* obj : M_subobjects) {
        fake_group.add(*obj);
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
    auto* current = M_subobjects[0];
    for (int i = 1; i < size(); ++i) {
        auto* next = M_subobjects[i];
        next->next_to(*current, vga2::e1, buff);
        current = next;
    }
    if (align != 0) {
        for (auto* obj : M_subobjects) {
            obj->align_to(*this, align);
        }
    }
    auto fake_group = Group();
    for (auto* obj : M_subobjects) {
        fake_group.add(*obj);
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
        row_groups[i / columns].add(*M_subobjects[i]);
        col_groups[i % columns].add(*M_subobjects[i]);
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
