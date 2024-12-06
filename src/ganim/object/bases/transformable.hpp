#ifndef GANIM_OBJECT_TRANSFORMABLE_HPP
#define GANIM_OBJECT_TRANSFORMABLE_HPP

/** @file
 * @brief Contains the @ref ganim::Transformable "Transformable" class.
 */

#include <memory>

#include "ganim/ga/conversions.hpp"

#include "animatable.hpp"

namespace ganim {

/** @brief Represents any object that can be transformed with rigid
 * transformations
 *
 * It's basically just a 3D PGA rotor with a bunch of utilities to make it
 * easier to apply operations to it.  Just call whatever functions you want on
 * it, then get the rotor with @ref Transformable::get_rotor.
 *
 * Also, note that in ganim, rotors transform in the opposite way than most
 * people do!  Most people use RAR† to do their transformations, whereas here
 * R†AR is used.  This means that to compose two rotors A and B such that A
 * happens before B, you must use A*B, not B*A.
 *
 * Most of the functions in this class return `*this` to allow for chaining.
 * If you want the chaining to work on subclasses, use the @ref
 * GANIM_TRANSFORMABLE_CHAIN_DECLS macro.
 */
class Transformable : public Animatable {
    public:
        /** @brief Default constuctor
         *
         * This just sets the rotor to the identity.
         */
        Transformable();
        /** @brief Constructor with a preset rotor */
        Transformable(const pga3::Even& rotor);
        /** @brief Resets the rotor to the identity */
        Transformable& reset();
        /** @brief Get the rotor that transforms this object */
        constexpr const pga3::Even& get_rotor() const {return M_rotor;}
        /** @brief Get the origin of this object, defined as `~R*e123*R` */
        pga3::Trivec get_origin() const;

        /** @brief Compose this rotor with another one.
         *
         * This will multiply the input rotor on the right of the current rotor,
         * so it will happen *after* the transformation that already exists.
         */
        Transformable& apply_rotor(const vga2::Even& rotor);
        /** @brief Compose this rotor with another one.
         *
         * This will multiply the input rotor on the right of the current rotor,
         * so it will happen *after* the transformation that already exists.
         */
        Transformable& apply_rotor(const vga3::Even& rotor);
        /** @brief Compose this rotor with another one.
         *
         * This will multiply the input rotor on the right of the current rotor,
         * so it will happen *after* the transformation that already exists.
         */
        Transformable& apply_rotor(const pga2::Even& rotor);
        /** @brief Compose this rotor with another one.
         *
         * This will multiply the input rotor on the right of the current rotor,
         * so it will happen *after* the transformation that already exists.
         */
        virtual Transformable& apply_rotor(const pga3::Even& rotor);

        /** @brief Move the origin of the object to a point. */
        Transformable& move_to(const pointlike auto& p)
            {return move_to(pointlike_to_pga3(p));}
        /** @brief Move the origin of the object to the point represented by the
         * 3D PGA trivector p.
         */
        Transformable& move_to(const pga3::Trivec& p);

        /** @brief Shift the object. */
        Transformable& shift(const pointlike auto& p)
            {return shift(pointlike_to_pga3(p));}
        /** @brief Shift the object by the 3D PGA trivector p.
         */
        Transformable& shift(const pga3::Trivec& p);

        /** @brief Rotate the object in the xy-plane about the origin */
        Transformable& rotate(double angle);
        /** @brief Rotate the object in the xy-plane about a given point */
        Transformable& rotate(const vga2::Vec& about_point, double angle);
        /** @brief Rotate the object in the xy-plane about a 2D PGA vector
         * intepreted as a 2D VGA vector
         */
        Transformable& rotate(const pga2::Vec& about_point, double angle);
        /** @brief Rotate the object in a given plane about the origin */
        Transformable& rotate(const vga3::Bivec& about_plane, double angle);
        /** @brief Rotate the object in the xy-plane about a given point
         *
         * Note that no normalization will be done.  If you want to just pass in
         * a point with the magnitude you want for your rotation, go ahead and
         * do so.  This is why the angle parameter default to one.
         */
        Transformable& rotate(
            const pga2::Bivec& about_point,
            double angle = 1
        );
        /** @brief Rotate the object about a given line
         *
         * Note that no normalization will be done.  If you want to just pass in
         * a line with the magnitude you want for your rotation, go ahead and do
         * so.  This is why the angle parameter default to one.
         */
        Transformable& rotate(
            const pga3::Bivec& about_line,
            double angle = 1
        );

        /** @brief Copy the object for the sake of transformations */
        std::unique_ptr<Transformable> anim_copy() const
            {return std::make_unique<Transformable>(*this);}
        /** @brief Interpolate between two Transformables */
        virtual void interpolate(
            const Transformable& start,
            const Transformable& end,
            double t
        );

    private:
        pga3::Even M_rotor = 1;
};

}

/** @brief Defines the @ref ganim::Transformable "Transformable" chainable
 * functions in a subclass to allow chaining in the subclass as well
 *
 * To use it, just call this macro in the public part of the class declaration,
 * where the Type parameter is the name of the subclass.
 */
#define GANIM_TRANSFORMABLE_CHAIN_DECLS(Type) \
    Type& reset() \
        {Transformable::reset(); return *this;} \
    Type& apply_rotor(const vga2::Even& rotor) \
        {Transformable::apply_rotor(rotor); return *this;} \
    Type& apply_rotor(const vga3::Even& rotor) \
        {Transformable::apply_rotor(rotor); return *this;} \
    Type& apply_rotor(const pga2::Even& rotor) \
        {Transformable::apply_rotor(rotor); return *this;} \
    Type& apply_rotor(const pga3::Even& rotor) \
        {Transformable::apply_rotor(rotor); return *this;} \
    Type& move_to(const pointlike auto& p) \
        {Transformable::move_to(p); return *this;} \
    Type& shift(const pointlike auto& p) \
        {Transformable::shift(p); return *this;} \
    Type& rotate(double angle) \
        {Transformable::rotate(angle); return *this;} \
    Type& rotate(const vga2::Vec& about_point, double angle) \
        {Transformable::rotate(about_point, angle); return *this;} \
    Type& rotate(const pga2::Vec& about_point, double angle) \
        {Transformable::rotate(about_point, angle); return *this;} \
    Type& rotate(const vga3::Bivec& about_plane, double angle) \
        {Transformable::rotate(about_plane, angle); return *this;} \
    Type& rotate(const pga2::Bivec& about_point, double angle = 1) \
        {Transformable::rotate(about_point, angle); return *this;} \
    Type& rotate(const pga3::Bivec& about_line, double angle = 1) \
        {Transformable::rotate(about_line, angle); return *this;} \

#endif
