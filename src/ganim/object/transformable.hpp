#ifndef GANIM_OBJECT_TRANSFORMABLE_H
#define GANIM_OBJECT_TRANSFORMABLE_H

/** @file
 * @brief Contains the @ref ganim::Transformable "Transformable" class.
 */

#include "ganim/ga/vga2.hpp"
#include "ganim/ga/vga3.hpp"
#include "ganim/ga/pga2.hpp"
#include "ganim/ga/pga3.hpp"

namespace ganim {

/** @brief Represents any object that can be transformed with rigid
 * transformations
 *
 * It's basically just a 3D PGA rotor with a bunch of utilities to make it
 * easier to apply operations to it.  Just call whatever functions you want on
 * it, then get the rotor with @ref Transformable::get_rotor.  Note that because
 * some transformable types want to do more when a rotor is applied, there is a
 * virtual function @ref Transformable::on_apply_rotor that you can overload if
 * you wish.
 *
 * Note that unlike in manim, the center of an object is not considered to be
 * the center of the object's bounding box.  Instead, it's just whatever the
 * identity is brought to by the object's rotor.  This definition of "center"
 * is what is used in the documentation here.
 *
 * Also, note that in ganim, rotors transform in the opposite way than most
 * people do!  Most people use RAR† to do their transformations, whereas here
 * R†AR is used.  This means that to compose two rotors A and B such that A
 * happens before B, you must use A*B, not B*A.
 */
class Transformable {
    public:
        /** @brief Default constuctor
         *
         * This just sets the rotor to the identity.
         */
        Transformable();
        /** @brief Constructor with a preset rotor */
        Transformable(const pga3::Even& rotor);
        /** @brief Resets the rotor to the identity */
        void reset();
        /** @brief Get the rotor that transforms this object */
        constexpr const pga3::Even& get_rotor() const {return M_rotor;}

        /** @brief Compose this rotor with another one.
         *
         * This will multiply the input rotor on the right of the current rotor,
         * so it will happen *after* the transformation that already exists.
         * This function will also pass the rotor parameter to @ref
         * on_apply_rotor.
         */
        void apply_rotor(const pga3::Even& rotor);

        /** @brief Move the center of the object to the point represented by the
         * 2D VGA vector p.
         */
        void move_to(const vga2::Vector& p);
        /** @brief Move the center of the object to the point represented by the
         * 3D VGA vector p.
         */
        void move_to(const vga3::Vector& p);
        /** @brief Move the center of the object to the point represented by the
         * 2D PGA vector p interpreted as a 2D VGA vector.
         */
        void move_to(const pga2::Vector& p);
        /** @brief Move the center of the object to the point represented by the
         * 3D PGA vector p interpreted as a 3D VGA vector.
         */
        void move_to(const pga3::Vector& p);
        /** @brief Move the center of the object to the point represented by the
         * 2D PGA bivector p.
         */
        void move_to(const pga2::Bivector& p);
        /** @brief Move the center of the object to the point represented by the
         * 3D PGA trivector p.
         */
        void move_to(const pga3::Trivector& p);

        /** @brief Shift the object by the 2D VGA vector p. */
        void shift(const vga2::Vector& p);
        /** @brief Shift the object by the 3D VGA vector p. */
        void shift(const vga3::Vector& p);
        /** @brief Shift the object by the vector represented by the 2D PGA
         * vector p interpreted as a 2D VGA vector.
         */
        void shift(const pga2::Vector& p);
        /** @brief Shift the object by the vector represented by the 3D PGA
         * vector p interpreted as a 3D VGA vector.
         */
        void shift(const pga3::Vector& p);
        /** @brief Shift the object by the 2D PGA ideal bivector p.
         *
         * Note that this function assumes that `p` is a point at infinity!
         * Shifting by points doesn't really make sense, but shifting by a point
         * at infinity does.
         */
        void shift(const pga2::Bivector& p);
        /** @brief Shift the object by the 3D PGA ideal trivector p.
         *
         * Note that this function assumes that `p` is a point at infinity!
         * Shifting by points doesn't really make sense, but shifting by a point
         * at infinity does.
         */
        void shift(const pga3::Trivector& p);

        /** @brief Rotate the object in the xy-plane about the origin */
        void rotate(double angle);
        /** @brief Rotate the object in the xy-plane about a given point */
        void rotate(const vga2::Vector& about_point, double angle);
        /** @brief Rotate the object in the xy-plane about a 2D PGA vector
         * intepreted as a 2D VGA vector
         */
        void rotate(const pga2::Vector& about_point, double angle);
        /** @brief Rotate the object in a given plane about the origin */
        void rotate(const vga3::Bivector& about_plane, double angle);
        /** @brief Rotate the object in the xy-plane about a given point
         *
         * Note that no normalization will be done.  If you want to just pass in
         * a point with the magnitude you want for your rotation, go ahead and
         * do so.  This is why the angle parameter default to one.
         */
        void rotate(const pga2::Bivector& about_point, double angle = 1);
        /** @brief Rotate the object about a given line
         *
         * Note that no normalization will be done.  If you want to just pass in
         * a line with the magnitude you want for your rotation, go ahead and do
         * so.  This is why the angle parameter default to one.
         */
        void rotate(const pga3::Bivector& about_line, double angle = 1);

    private:
        /** @brief Allows for subclasses to have special behavior when a rotor
         * is applied.
         *
         * This function will be called by @ref apply_rotor and, by proxy, any
         * of the other functions that modifies the rotor.  The default
         * implementation is to just do nothing.
         */
        virtual void on_apply_rotor(const pga3::Even&) {}
        pga3::Even M_rotor = 1;
};

}

#endif
