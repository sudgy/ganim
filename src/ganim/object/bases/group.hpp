#ifndef GANIM_OBJECT_COMPOUND_OBJECT_HPP
#define GANIM_OBJECT_COMPOUND_OBJECT_HPP

/** @file
 * @brief Contains the @ref ganim::Group "Group" class
 */

#include "object.hpp"

namespace ganim {

class Group;

template <typename T>
concept normal_input_range =
    std::ranges::input_range<T> and
    !requires(T& group, ObjectPtr<Object> object) {
        group->add(object);
    } and
    !requires(T& group, ObjectPtr<Object> object) {
        group.add(object);
    };

struct ArrangeArgs {
    double buff = 0.25;
    vga2::Vec align = 0*vga2::e1;
};
/** @brief The class for objects that contains other objects
 *
 * You can put the same object in multiple Groups and nothing will go wrong.
 * However, putting the same object in multiple Groups with draw_together can go
 * wrong because they'll get drawn multiple times.  You can put an object in
 * multiple Groups as long as draw_together is only true for one of them.
 *
 * This class is kind of like a container.  You can iterate in a range-based for
 * loop (although you'll be iterating over pointers, not references), you can
 * query its size, and you can get ObjectPtrs with the index operator.
 *
 * It is up to you to ensure that you don't add the same object to the same
 * group of any kind multiple times, even transitively.  Things might get weird
 * if you do.
 */
class Group : public Object {
    public:
        Group()=default;
        Group(const Group&)=default;
        Group(Group&&) noexcept=default;
        Group& operator=(const Group&)=default;
        Group& operator=(Group&&) noexcept=default;
        /*** @brief Add an object to this group */
        void add(ObjectPtr<Object> object);
        /** @brief Adds a range of objects to this group
         *
         * Even though groups are ranges, when adding groups to groups, we don't
         * want to add all the subobjects of the subgroup to this group, so
         * groups are explicitly disallowed from this function.
         *
         * @tparam R A range type that is not a Group of some kind
         */
        template <normal_input_range R>
        void add(R& range)
        {
            for (auto& object : range) {
                add(object);
            }
        }
        /** @brief Adds several objects to this group
         *
         * This just calls one of the other add functions on all the arguments.
         */
        template <typename... Ts> requires(sizeof...(Ts) > 1)
        void add(Ts&... objects)
        {
            (add(objects), ...);
        }
        /** @brief Remove an object from a group */
        void remove(Object& object);
        /** @brief Remove an object from a group */
        inline void remove(ObjectPtr<Object> object) {remove(*object);}
        /** @brief Remove a range of objects from a group */
        template <normal_input_range R>
        void remove(R& range)
        {
            for (auto& object : range) {
                remove(object);
            }
        }
        /** @brief Remove several objects from a group */
        template <typename... Ts> requires(sizeof...(Ts) > 1)
        void remove(Ts&... objects)
        {
            (remove(objects), ...);
        }

        /** @brief Returns this. */
        virtual Group* as_group() override final {return this;}
        virtual const Group* as_group() const override final {return this;}
        /** @brief Interpolate two groups.
         *
         * Interpolating groups is more confusing than normal since most of the
         * state of the group *shouldn't* be interpolated, and instead each
         * subobject needs to be interpolated.
         *
         * @throws std::invalid_argument If the size of *this, start, and end
         * are not the same, or if the position of subgroups in *this, start,
         * and end are not at the same indices.  If an exception is thrown, no
         * interpolation will have happened at all.
         */
        virtual void interpolate(
            const Animatable& start,
            const Animatable& end,
            double t
        ) override;
        ObjectPtr<Group> polymorphic_copy() const;

        virtual void draw(const Camera& camera) override;
        virtual bool is_visible() const override;
        virtual void draw_outline(const Camera& camera) override;
        virtual void set_outline(
            const Color& color,
            double thickness,
            bool shift_depth = false
        ) override;
        virtual void invalidate_outline() override;
        virtual Color get_outline_color() const override;
        virtual double get_outline_thickness() const override;
        virtual void set_peeling_depth_buffer(gl::Texture* texture) override;

        // I can't use the macro here because I'm overloading some of the
        // functions
        Group& reset()
            {Transformable::reset(); return *this;}
        Group& apply_rotor(const vga2::Even& rotor)
            {Transformable::apply_rotor(rotor); return *this;}
        Group& apply_rotor(const vga3::Even& rotor)
            {Transformable::apply_rotor(rotor); return *this;}
        Group& apply_rotor(const pga2::Even& rotor)
            {Transformable::apply_rotor(rotor); return *this;}
        Group& move_to(const pointlike auto& p)
            {Transformable::move_to(p); return *this;}
        Group& shift(const pointlike auto& p)
            {Transformable::shift(p); return *this;}
        Group& rotate(double angle)
            {Transformable::rotate(angle); return *this;}
        Group& rotate(double angle, const vga2::Vec& about_point)
            {Transformable::rotate(angle, about_point); return *this;}
        Group& rotate(double angle, const pga2::Vec& about_point)
            {Transformable::rotate(angle, about_point); return *this;}
        Group& rotate(double angle, const vga3::Bivec& about_plane)
            {Transformable::rotate(angle, about_plane); return *this;}
        Group& rotate(double angle, const pga2::Bivec& about_point)
            {Transformable::rotate(angle, about_point); return *this;}
        Group& rotate(double angle, const pga3::Bivec& about_line)
            {Transformable::rotate(angle, about_line); return *this;}
        Group& scale(double amount)
            {Object::scale(amount); return *this;}
        Group& scale(double amount, const pointlike auto& about_point)
            {Object::scale(amount, about_point); return *this;}
        virtual void set_fps(int fps) override;
        virtual Group& apply_rotor(const pga3::Even& rotor) override;
        virtual Group& set_color(Color color) override;
        virtual Group& push_color() override;
        virtual Group& push_color(Color color) override;
        virtual Group& pop_color() override;
        virtual Group& set_opacity(double opacity) override;
        virtual Group& scale(double amount, const pga3::Trivec& about_point)
            override;
        virtual Group& set_visible(bool visible) override;
        virtual void set_draw_fraction(double value) override;
        virtual Group& set_depth_z(double depth_z) override;
        virtual void set_creating(bool creating) override;
        virtual void set_noise_creating(double noise_creating) override;
        virtual void set_fixed_in_frame(bool fixed_in_frame) override;
        virtual void set_fixed_orientation(bool fixed_orientation) override;
        virtual Box get_true_bounding_box() const override;
        virtual Box get_logical_bounding_box() const override;
        virtual Box get_original_true_bounding_box() const override;
        virtual Box get_original_logical_bounding_box() const override;
        virtual void set_animating(bool animating) override;
        virtual bool is_animating() const override;
        /** @brief Set how far into one subobject to draw before starting to
         * draw the next one
         *
         * This controls how the draw fraction is propogated to subobjects.  Set
         * it to one to make each subobject get drawn completely before drawing
         * the next one, and set it to zero in make them all get drawn at the
         * same time.  It must be between zero and one.  If it's not, an
         * exception will be thrown.
         */
        void set_draw_subobject_ratio(double ratio);
        /** @brief See how far into one subobject to draw before starting to
         * draw the next one
         *
         * @see set_draw_subobject_ratio
         */
        double get_draw_subobject_ratio() const;
        /** @brief Make the subobjects of this object be drawn at the same time
         * for the sake of outlines
         *
         * This affects when outlines are drawn.  After calling this function,
         * all outlines of the objects will be drawn before the objects
         * themselves.  This is good for things like text where you don't want
         * the outline of one letter to cover up another letter.
         */
        void draw_together() {M_draw_together = true;}
        /** @brief Make the subobjects of this object not be drawn at the same
         * time for the sake of outlines
         *
         * This affects when outlines are drawn.  After calling this function,
         * the outline of each object will be drawn immediately before the
         * object itself, causing the outline of one subobject to cover up other
         * objects.  This is good for situations where the subobjects are
         * visibly distinct.
         */
        void draw_separately() {M_draw_together = false;}
        /** @brief Check whether or not the subobjects will be drawn at the same
         * time for the sake of outlines
         *
         * @see draw_together
         * @see draw_separately
         */
        bool drawing_together() const {return M_draw_together;}

        auto begin() {return M_subobjects.begin();}
        auto end() {return M_subobjects.end();}
        auto begin() const {return M_subobjects.begin();}
        auto end() const {return M_subobjects.end();}
        auto cbegin() const {return M_subobjects.begin();}
        auto cend() const {return M_subobjects.end();}
        int size() const {return static_cast<int>(M_subobjects.size());}
        ObjectPtr<Object> operator[](int index)
            {return M_subobjects[index];}
        ObjectPtr<const Object> operator[](int index) const
            {return M_subobjects[index];}
        /** @brief Remove all subobjects from this object */
        void clear() {M_subobjects.clear(); M_new_subobjects.clear();}

        /** @brief Get a range of subobjects
         *
         * This will produce a new group consisting of subobjects in the range
         * [i1, i2).
         */
        ObjectPtr<Group> range(int i1, int i2);
        /** @brief Get a range of subobjects
         *
         * This will produce a new group consisting of subobjects starting at i
         * and going to the end.
         */
        ObjectPtr<Group> range(int i);

        /** @brief Align this group to a point, but using a subobject to do the
         * aligning rather than the whole group
         *
         * @param index The index of the subobject to do the aligning
         * @param point The point to align the subobject to
         * @param direction The direction on the bounding box of the
         * subobject to align to the point
         */
        Group& align_by_subobject(
            int index,
            const pga3::Trivec& point,
            const pga3::Trivec& direction
        );
        /** @brief Align this group to an object, but using a subobject to do
         * the aligning rather than the whole group
         *
         * @param index The index of the subobject to do the aligning
         * @param object The object to align the subobject to
         * @param direction The direction on the bounding box of the
         * subobject to align to the point
         */
        Group& align_by_subobject(
            int index,
            const Object& object,
            const pga3::Trivec& direction
        );
        /** @brief Align this group to a point, but using a subobject to do the
         * aligning rather than the whole group
         *
         * @param index The index of the subobject to do the aligning
         * @param point The point to align the subobject to
         * @param direction The direction on the bounding box of the
         * subobject to align to the point
         */
        Group& align_by_subobject(
            int index,
            const pointlike auto& point,
            const pointlike auto& direction
        )
        {
            return align_by_subobject(
                index,
                pointlike_to_pga3(point),
                pointlike_to_pga3(direction)
            );
        }
        /** @brief Align this group to an object, but using a subobject to do
         * the aligning rather than the whole group
         *
         * @param index The index of the subobject to do the aligning
         * @param object The object to align the subobject to
         * @param direction The direction on the bounding box of the
         * subobject to align to the point
         */
        Group& align_by_subobject(
            int index,
            const Object& object,
            const pointlike auto& direction
        )
        {
            return align_by_subobject(
                index,
                object,
                pointlike_to_pga3(direction)
            );
        }
        /** @brief Align this group to an object, but using a subobject to do
         * the aligning rather than the whole group
         *
         * These two-direction overloads work the same way as @ref
         * Object::align_to.
         */
        Group& align_by_subobject(
            int index,
            const Object& object,
            const pointlike auto& direction1,
            const pointlike auto& direction2
        )
        {
            align_by_subobject(index, object, direction1);
            return align_by_subobject(index, object, direction2);
        }
        /** @brief Align this group to a point, but using a subobject to do the
         * aligning rather than the whole group
         *
         * These two-direction overloads work the same way as @ref
         * Object::align_to.
         */
        Group& align_by_subobject(
            int index,
            const pointlike auto& point,
            const pointlike auto& direction1,
            const pointlike auto& direction2
        )
        {
            align_by_subobject(index, point, direction1);
            return align_by_subobject(index, point, direction2);
        }

        /** @brief Arrange the subobjects in this group in a vertical stack */
        Group& arrange_down(ArrangeArgs args = {});
        /** @brief Arrange the subobjects in this group in a horizontal stack */
        Group& arrange_right(ArrangeArgs args = {});
        /** @brief Arrange the subobjects in this group in a grid */
        Group& arrange_in_grid(
            int columns,
            ArrangeArgs hor_args = {},
            ArrangeArgs ver_args = {}
        );

        /** @private */
        std::vector<ObjectPtr<Object>> take_new_subobjects()
        {
            return std::move(M_new_subobjects);
        }

    protected:
        bool propagate() const {return M_propogate;}

    private:
        virtual Group* polymorphic_copy_impl() const override;

        std::vector<ObjectPtr<Object>> M_subobjects;
        std::vector<ObjectPtr<Object>> M_new_subobjects;
        double M_ratio = 1;
        double M_outline_thickness = 0;
        Color M_outline_color = Color("#000000");
        bool M_propogate = true;
        bool M_draw_together = false;
};

/** @brief Make a group in an ObjectPtr.
 *
 * You can pass any number of objects in to this function and they will all be
 * added to the newly-created group.
 */
template <typename... Ts>
ObjectPtr<Group> make_group(Ts&&... objects)
{
    auto result = ObjectPtr<Group>();
    (result->add(std::forward<Ts>(objects)), ...);
    return result;
}

}

#endif
