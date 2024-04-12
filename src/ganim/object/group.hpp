#ifndef GANIM_OBJECT_GROUP_HPP
#define GANIM_OBJECT_GROUP_HPP

/** @file
 * @brief Contains the @ref ganim::Group "Group" class
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
class Group : public Object {
    public:
        template <typename... Ts>
        explicit Group(Ts&... objects)
        {
            (add(objects), ...);
        }
        /*** @brief Add an object to this group */
        void add(Object& object);
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
        Group& rotate(const vga2::Vector& about_point, double angle)
            {Transformable::rotate(about_point, angle); return *this;}
        Group& rotate(const pga2::Vector& about_point, double angle)
            {Transformable::rotate(about_point, angle); return *this;}
        Group& rotate(const vga3::Bivector& about_plane, double angle)
            {Transformable::rotate(about_plane, angle); return *this;}
        Group& rotate(const pga2::Bivector& about_point, double angle = 1)
            {Transformable::rotate(about_point, angle); return *this;}
        Group& rotate(const pga3::Bivector& about_line, double angle = 1)
            {Transformable::rotate(about_line, angle); return *this;}
        Group& scale(double amount)
            {Object::scale(amount); return *this;}
        Group& scale(const pointlike auto& about_point, double amount)
            {Object::scale(about_point, amount); return *this;}
        virtual Group& apply_rotor(const pga3::Even& rotor) override;
        virtual Group& set_color(Color color) override;
        virtual Group& set_color_with_alpha(Color color) override;
        virtual Group& set_opacity(double opacity) override;
        virtual Group& scale(const pga3::Trivector& about_point, double amount)
            override;
        virtual Group& set_visible(bool visible) override;
        virtual void set_draw_fraction(double value) override;
        virtual void set_creating(bool creating) override;
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

    private:
        std::vector<Object*> M_subobjects;
        bool M_propogate = true;
        double M_ratio = 1;
};

}

#endif
