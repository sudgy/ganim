#ifndef GANIM_OBJECT_COMPOUND_OBJECT_HPP
#define GANIM_OBJECT_COMPOUND_OBJECT_HPP

/** @file
 * @brief Contains the @ref ganim::Group "Group" class
 */

#include "object.hpp"

namespace ganim {

class Group;
struct ArrangeArgs {
    double buff = 0.25;
    vga2::Vec align = 0*vga2::e1;
};
/** @brief The class for objects that contains other objects
 *
 * Note that this class does not own its subobjects!  You need to make sure to
 * own them elsewhere (such as in a subclass).  This means that you can put the
 * same object in multiple Groups and nothing will go wrong.  However, putting
 * the same object in multiple Groups with draw_together can go wrong because
 * they'll get drawn multiple times.  You can put an object in multiple Groups
 * as long as draw_together is only true for one of them.
 *
 * This class is kind of like a container.  You can iterate in a range-based for
 * loop (although you'll be iterating over pointers, not references), you can
 * query its size, and you can get object references with the index operator.
 *
 * It is up to you to ensure that you don't add the same object to the same
 * group of any kind multiple times, even transitively.  Things might get weird
 * if you do.
 */
class Group : public Object {
    public:
        template <typename... Ts>
        explicit Group(Ts&... objects)
        {
            (add(objects), ...);
        }
        /*** @brief Add an object to this group */
        virtual void add(Object& object);
        /** @brief Adds a range of objects to this group
         *
         * Even though groups are ranges, when adding groups to groups, we don't
         * want to add all the subobjects of the subgroup to this group, so
         * groups are explicitly disallowed from this function.
         *
         * @tparam R A range type that is not a Group of some kind
         */
        template <std::ranges::input_range R>
            requires(!std::convertible_to<R&, Group&>)
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

        /** @brief Returns this. */
        virtual Group* as_group() override final {return this;}
        virtual const Group* as_group() const override final {return this;}
        /** @brief Copy a group for the sake of animations.
         *
         * This copies a group, including all of its subobjects.
         */
        std::unique_ptr<Group> anim_copy() const;
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
            const Group& start,
            const Group& end,
            double t
        );
        /** @brief Interpolate between two groups.
         *
         * If the inputs aren't actually groups, it will throw
         * std::invalid_argument.
         */
        virtual void interpolate(
            const Transformable& start,
            const Transformable& end,
            double t
        ) override;
        /** @brief Interpolate between two groups.
         *
         * If the inputs aren't actually groups, it will throw
         * std::invalid_argument.
         */
        virtual void interpolate(
            const Object& start,
            const Object& end,
            double t
        ) override;

        virtual void draw(const Camera& camera) override;
        virtual bool is_visible() const override;
        virtual void draw_outline(const Camera& camera) override;
        virtual void set_outline(const Color& color, double thickness) override;
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
        Group& rotate(const vga2::Vec& about_point, double angle)
            {Transformable::rotate(about_point, angle); return *this;}
        Group& rotate(const pga2::Vec& about_point, double angle)
            {Transformable::rotate(about_point, angle); return *this;}
        Group& rotate(const vga3::Bivec& about_plane, double angle)
            {Transformable::rotate(about_plane, angle); return *this;}
        Group& rotate(const pga2::Bivec& about_point, double angle = 1)
            {Transformable::rotate(about_point, angle); return *this;}
        Group& rotate(const pga3::Bivec& about_line, double angle = 1)
            {Transformable::rotate(about_line, angle); return *this;}
        Group& scale(double amount)
            {Object::scale(amount); return *this;}
        Group& scale(const pointlike auto& about_point, double amount)
            {Object::scale(about_point, amount); return *this;}
        virtual void set_fps(int fps) override;
        virtual Group& apply_rotor(const pga3::Even& rotor) override;
        virtual Group& set_color(Color color) override;
        virtual Group& set_color_with_alpha(Color color) override;
        virtual Group& set_opacity(double opacity) override;
        virtual Group& scale(const pga3::Trivec& about_point, double amount)
            override;
        virtual Group& set_visible(bool visible) override;
        virtual void set_draw_fraction(double value) override;
        virtual void set_creating(bool creating) override;
        virtual void set_noise_creating(double noise_creating) override;
        virtual void set_fixed_in_frame(bool fixed_in_frame) override;
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
        double get_draw_subobject_ratio() const;
        void draw_together() {M_draw_together = true;}
        void draw_separately() {M_draw_together = false;}
        bool drawing_together() const {return M_draw_together;}

        auto begin() {return M_subobjects.begin();}
        auto end() {return M_subobjects.end();}
        auto begin() const {return M_subobjects.begin();}
        auto end() const {return M_subobjects.end();}
        auto cbegin() const {return M_subobjects.begin();}
        auto cend() const {return M_subobjects.end();}
        int size() const {return static_cast<int>(M_subobjects.size());}
        Object& operator[](int index) {return *M_subobjects[index];}
        const Object& operator[](int index) const {return *M_subobjects[index];}

        Group range(int i1, int i2);
        Group range(int i);

        Group& align_by_subobject(
            int index,
            const pga3::Trivec& point,
            const pga3::Trivec& direction
        );
        Group& align_by_subobject(
            int index,
            const Object& object,
            const pga3::Trivec& direction
        );
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

        Group& arrange_down(ArrangeArgs args = {});
        Group& arrange_right(ArrangeArgs args = {});
        Group& arrange_in_grid(
            int columns,
            ArrangeArgs hor_args = {},
            ArrangeArgs ver_args = {}
        );

    private:
        std::vector<Object*> M_subobjects;
        double M_ratio = 1;
        double M_outline_thickness = 0;
        Color M_outline_color = Color("#000000");
        bool M_propogate = true;
        bool M_draw_together = false;
};

}

#endif
