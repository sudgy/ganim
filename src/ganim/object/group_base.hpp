#ifndef GANIM_OBJECT_GROUP_BASE_HPP
#define GANIM_OBJECT_GROUP_BASE_HPP

/** @file
 * @brief Contains the @ref ganim::GroupBase "GroupBase" class
 */

#include "object.hpp"

namespace ganim {

/** @brief An object that contains other objects
 *
 * Note that unlike in manim, this class is purely organizational!  All it will
 * do is things like shifting all of its subobjects when you shift the group.
 * You can still mess with the individual objects all you want, even if they're
 * in a group.  In fact, groups don't even own the objects that are in a group.
 * You need to own them somewhere else (such as in a subclass).
 *
 * This class is kind of like a container.  You can iterate in a range-based for
 * loop (although you'll be iterating over pointers, not references), you can
 * query its size, and you can get object references with the index operator.
 *
 * It is up to you to ensure that you don't add the same object to a group
 * multiple times, even transitively.  Things might get weird if you do.
 */
class GroupBase : public Object {
    private:
        friend class Cluster;
        friend class Group;
        GroupBase()=default;

    public:
        /*** @brief Add an object to this group */
        void add(Object& object);
        /** @brief Adds a range of objects to this group
         *
         * Even though groups are ranges, when adding groups to groups, we don't
         * want to add all the subobjects of the subgroup to this group, so
         * groups are explicitly disallowed from this function.
         *
         * @tparam R A range type that is not a GroupBase of some kind
         */
        template <std::ranges::input_range R>
            requires(!std::convertible_to<R&, GroupBase&>)
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
        virtual GroupBase* as_group() override final {return this;}
        virtual const GroupBase* as_group() const override final {return this;}
        /** @brief Copy a group for the sake of animations.
         *
         * This copies a group, including all of its subobjects.
         */
        std::unique_ptr<GroupBase> anim_copy() const;
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
            const GroupBase& start,
            const GroupBase& end,
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

        // I can't use the macro here because I'm overloading some of the
        // functions
        GroupBase& reset()
            {Transformable::reset(); return *this;}
        GroupBase& apply_rotor(const vga2::Even& rotor)
            {Transformable::apply_rotor(rotor); return *this;}
        GroupBase& apply_rotor(const vga3::Even& rotor)
            {Transformable::apply_rotor(rotor); return *this;}
        GroupBase& apply_rotor(const pga2::Even& rotor)
            {Transformable::apply_rotor(rotor); return *this;}
        GroupBase& move_to(const pointlike auto& p)
            {Transformable::move_to(p); return *this;}
        GroupBase& shift(const pointlike auto& p)
            {Transformable::shift(p); return *this;}
        GroupBase& rotate(double angle)
            {Transformable::rotate(angle); return *this;}
        GroupBase& rotate(const vga2::Vector& about_point, double angle)
            {Transformable::rotate(about_point, angle); return *this;}
        GroupBase& rotate(const pga2::Vector& about_point, double angle)
            {Transformable::rotate(about_point, angle); return *this;}
        GroupBase& rotate(const vga3::Bivector& about_plane, double angle)
            {Transformable::rotate(about_plane, angle); return *this;}
        GroupBase& rotate(const pga2::Bivector& about_point, double angle = 1)
            {Transformable::rotate(about_point, angle); return *this;}
        GroupBase& rotate(const pga3::Bivector& about_line, double angle = 1)
            {Transformable::rotate(about_line, angle); return *this;}
        GroupBase& scale(double amount)
            {Object::scale(amount); return *this;}
        GroupBase& scale(const pointlike auto& about_point, double amount)
            {Object::scale(about_point, amount); return *this;}
        virtual GroupBase& apply_rotor(const pga3::Even& rotor) override;
        virtual GroupBase& set_color(Color color) override;
        virtual GroupBase& set_color_with_alpha(Color color) override;
        virtual GroupBase& set_opacity(double opacity) override;
        virtual GroupBase& scale(const pga3::Trivector& about_point, double amount)
            override;
        virtual GroupBase& set_visible(bool visible) override;
        virtual void set_draw_fraction(double value) override;
        virtual void set_creating(bool creating) override;
        virtual void set_noise_creating(double noise_creating) override;
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

        auto begin() {return M_subobjects.begin();}
        auto end() {return M_subobjects.end();}
        auto begin() const {return M_subobjects.begin();}
        auto end() const {return M_subobjects.end();}
        auto cbegin() const {return M_subobjects.begin();}
        auto cend() const {return M_subobjects.end();}
        int size() const {return static_cast<int>(M_subobjects.size());}
        Object& operator[](int index) {return *M_subobjects[index];}
        const Object& operator[](int index) const {return *M_subobjects[index];}

    private:
        std::vector<Object*> M_subobjects;
        bool M_propogate = true;
        double M_ratio = 1;
};

}

#endif
