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
            Object& set_color_with_alpha(Color color);
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

            Object& scale(double amount);
            Object& scale(const vga2::Vector& about_point, double amount);
            Object& scale(const vga3::Vector& about_point, double amount);
            Object& scale(const pga2::Vector& about_point, double amount);
            Object& scale(const pga3::Vector& about_point, double amount);
            Object& scale(const pga2::Bivector& about_point, double amount);
            Object& scale(const pga3::Trivector& about_point, double amount);
            double get_scale() const;

            GANIM_TRANSFORMABLE_CHAIN_DECLS(Object);

        private:
            virtual void on_scale(
                const pga3::Trivector& about_point,
                double amount
            ) {(void)about_point; (void)amount;}
            virtual void on_color_changed(Color new_color) {(void)new_color;}
            virtual void transformable_on_animate() override final;
            virtual void transformable_on_animation_start() override final;
            virtual void transformable_update_animation(double t)override final;
            virtual void transformable_on_animation_end() override final;
            /** @brief Called by @ref transformable_on_animate */
            virtual void object_on_animate() {}
            /** @brief Called by @ref transformable_on_animation_start */
            virtual void object_on_animation_start() {}
            /** @brief Called by @ref transformable_update_animation */
            virtual void object_update_animation(double t) {(void)t;}
            /** @brief Called by @ref transformable_on_animation_end */
            virtual void object_on_animation_end() {}

            Color M_color = {255, 255, 255, 255};
            Color M_starting_color;
            Color M_ending_color;
            double M_scale = 1;
            double M_starting_scale = 1;
            double M_ending_scale = 1;
            pga3::Trivector M_scale_point;
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
