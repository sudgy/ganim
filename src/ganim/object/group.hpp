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

        using Object::apply_rotor;
        virtual Group& apply_rotor(const pga3::Even& rotor) override;
        virtual Group& set_color(Color color) override;
        virtual Group& set_color_with_alpha(Color color) override;
        virtual Group& set_opacity(double opacity) override;
        using Object::scale;
        virtual Group& scale(const pga3::Trivector& about_point, double amount)
            override;

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
};

}

#endif
