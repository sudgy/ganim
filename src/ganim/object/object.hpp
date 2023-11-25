#ifndef GANIM_OBJECT_OBJECT_HPP
#define GANIM_OBJECT_OBJECT_HPP

/** @file
 * @brief Contains the @ref ganim::Object "Object" class
 */

#include "transformable.hpp"

#include "ganim/color.hpp"

namespace ganim {
    /** @brief The base class for objects that have some kind of extent
     *
     * Most of the functions in this class return `*this` to allow for chaining.
     * If you want the chaining to work on subclasses, use the @ref
     * GANIM_OBJECT_CHAIN_DECLS macro.
     */
    class Object : public Transformable {
        public:
            /** @brief Set the color of this object, ignoring alpha. */
            Object& set_color(Color color);
            /** @brief Set the color of this object, including alpha. */
            virtual Object& set_color_with_alpha(Color color);
            /** @brief Set the opacity of this object
             *
             * Note that this class doesn't actually keep track of an opacity
             * value separately from the opacity in the color.
             *
             * @param opacity The opacity to change to, from 0 to 1.  0 is fully
             * transparent, while 1 is fully opaque.
             */
            Object& set_opacity(double opacity);
            /** @brief Get the color of this object, including alpha. */
            Color get_color() const {return M_color;}

            /** @brief Scale the object about its center */
            Object& scale(double amount);
            /** @brief Scale the object about a particular point */
            Object& scale(const pointlike auto& p, double amount)
                {return scale(pointlike_to_pga3(p), amount);}
            /** @brief Scale the object about a particular point */
            virtual Object& scale(
                const pga3::Trivector& about_point,
                double amount
            );
            /** @brief Get the amount that this object is scaled */
            double get_scale() const;
            /** @brief Set whether this object is visible */
            virtual Object& set_visible(bool visible);
            /** @brief See whether this object is visible */
            bool is_visible() const {return M_visible;}

            /** @brief Set how much of the object to draw.
             *
             * This is used for things like the @ref ganim::create "create"
             * animation.  Note that this doesn't actually affect anything other
             * than the value returned by @ref get_draw_fraction!  It's up to
             * subclasses to use this value to do something interesting.
             */
            virtual void set_draw_fraction(double value)
                {M_draw_fraction = value;}
            /** @brief Get how much of the object to draw.
             *
             * This is used for things like the @ref ganim::create "create"
             * animation.  Note that this doesn't actually affect anything other
             * than the value returned by @ref get_draw_fraction!  It's up to
             * subclasses to use this value to do something interesting.
             */
            double get_draw_fraction() const {return M_draw_fraction;}

            /** @brief Copy the object for the sake of transformations */
            std::unique_ptr<Object> anim_copy() const
                {return std::make_unique<Object>(*this);}
            /** @brief Interpolate between two Objects */
            void interpolate(const Object& start, const Object& end, double t);

            GANIM_TRANSFORMABLE_CHAIN_DECLS(Object);

        private:

            Color M_color = {255, 255, 255, 255};
            double M_scale = 1;
            double M_draw_fraction = 1;
            bool M_visible = false;
    };
}

/** @brief Defines the @ref ganim::Object "Object" chainable functions in a
 * subclass to allow chaining in the subclass as well
 *
 * To use it, just call this macro in the public part of the class declaration,
 * where the Type parameter is the name of the subclass.  Note that it will
 * automatically define the @ref ganim::Transformable "Transformable" chainable
 * functions as well, so you don't need to use both @ref
 * GANIM_TRANSFORMABLE_CHAIN_DECLS and this macro.
 */
#define GANIM_OBJECT_CHAIN_DECLS(Type) \
    GANIM_TRANSFORMABLE_CHAIN_DECLS(Type) \
    Type& set_visible(bool visible) \
        {Object::set_visible(visible); return *this;} \
    Type& set_color(Color color) \
        {Object::set_color(color); return *this;} \
    Type& set_color_with_alpha(Color color) \
        {Object::set_color_with_alpha(color); return *this;} \
    Type& set_opacity(double opacity) \
        {Object::set_opacity(opacity); return *this;} \
    Type& scale(double amount) \
        {Object::scale(amount); return *this;} \
    Type& scale(const vga2::Vector& about_point, double amount) \
        {Object::scale(about_point, amount); return *this;} \
    Type& scale(const vga3::Vector& about_point, double amount) \
        {Object::scale(about_point, amount); return *this;} \
    Type& scale(const pga2::Vector& about_point, double amount) \
        {Object::scale(about_point, amount); return *this;} \
    Type& scale(const pga3::Vector& about_point, double amount) \
        {Object::scale(about_point, amount); return *this;} \
    Type& scale(const pga2::Bivector& about_point, double amount) \
        {Object::scale(about_point, amount); return *this;} \
    Type& scale(const pga3::Trivector& about_point, double amount) \
        {Object::scale(about_point, amount); return *this;}

#endif
