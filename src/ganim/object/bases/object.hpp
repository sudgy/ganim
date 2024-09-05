#ifndef GANIM_OBJECT_OBJECT_HPP
#define GANIM_OBJECT_OBJECT_HPP

/** @file
 * @brief Contains the @ref ganim::Object "Object" class
 */

#include "transformable.hpp"

#include "ganim/color.hpp"
#include "ganim/util/box.hpp"
#include "ganim/scene/camera.hpp"

namespace ganim {
    /** @brief The base class for objects that have some kind of extent
     *
     * Most of the functions in this class return `*this` to allow for chaining.
     * If you want the chaining to work on subclasses, use the @ref
     * GANIM_OBJECT_CHAIN_DECLS macro.
     */
    class Object : public Transformable {
        public:
            /** @brief Draw this object onto the screen
             *
             * @param camera The camera being used to draw onto the screen.  You
             * should use this to get the x and y scale values and the camera's
             * position/orientation.
             */
            virtual void draw(const Camera& camera)=0;
            /** @brief Draw the outline of this object onto the screen
             *
             * Drawing the object and the outline are separated so that you can
             * draw several outlines of the objects in a group before drawing
             * the objects themselves.
             *
             * @param camera The camera being used to draw onto the screen.  You
             * should use this to get the x and y scale values and the camera's
             * position/orientation.
             */
            virtual void draw_outline(const Camera& camera)=0;
            /** @brief Set the outline color and thickness to use when drawing
             * the outline of this object
             *
             * Use a value of 0.0 for the thickness to disable the outline.
             *
             * @param color The color of the outline.
             * @param thickness The width, in ganim units, of the outline.
             */
            virtual void set_outline(const Color& color, double thickness)=0;
            virtual void invalidate_outline()=0;
            virtual Color get_outline_color() const=0;
            virtual double get_outline_thickness() const=0;

            /** @brief Set the color of this object, ignoring alpha. */
            virtual Object& set_color(Color color);
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
            virtual Object& set_opacity(double opacity);
            /** @brief Get the color of this object, including alpha. */
            Color get_color() const {return M_color;}

            /** @brief Scale the object about its origin */
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
            virtual bool is_visible() const {return M_visible;}

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
            std::unique_ptr<Object> anim_copy() const;
            using Transformable::interpolate;
            /** @brief Interpolate between two Objects */
            virtual void interpolate(
                const Object& start,
                const Object& end,
                double t
            );

            /** @brief Set whether or not this object is being created.
             *
             * Used internally by things like the create animation.
             */
            virtual void set_creating(bool creating) {M_creating = creating;}
            /** @brief Set whether or not this object is being noise created,
             * and the amount that the noise should affect the creation.
             *
             * Used internally by things like the noise create animation.
             */
            virtual void set_noise_creating(double noise_creating)
                {M_noise_creating = noise_creating;}
            /** @brief Determine whether or not this object is being created.
             *
             * Used internally by things like the create animation.
             */
            bool is_creating() const {return M_creating;}
            /** @brief Determine whether or not this object is being noise
             * created, and the amount that the noise should affect the
             * creation.
             *
             * Used internally by things like the noise create animation.
             */
            double noise_creating() const {return M_noise_creating;}
            /** @brief Get the true bounding box of this object
             *
             * This bounding box must be big enough that the entirety of the
             * object is inside it.  Note that this function returns the
             * transformed bounding box.  Unless you know what you're doing, you
             * probably shouldn't override this function.
             */
            virtual Box get_true_bounding_box() const;
            /** @brief Get the logical bounding box of this object
             *
             * Sometimes, the thing that "feels" like the bounding box of an
             * object isn't the actual bounding box.  Note that this function
             * returns the transformed bounding box.  Unless you know what
             * you're doing, you probably shouldn't override this function.
             */
            virtual Box get_logical_bounding_box() const;
            /** @brief Get the true bounding box of this object, before any
             * transformations are applied.
             *
             * This bounding box must be big enough that the entirety of the
             * object is inside it.  This is the one function that you need to
             * override, and the other three bounding box functions will have
             * reasonable defaults based on it.
             */
            virtual Box get_original_true_bounding_box() const=0;
            /** @brief Get the logical bounding box of this object, before any
             * transformations are applied.
             *
             * Sometimes, the thing that "feels" like the bounding box of an
             * object isn't the actual bounding box.  If your object is like
             * this, override this function to return this "logical" bounding
             * box.  It defaults to just returning @ref
             * get_original_true_bounding_box.
             */
            virtual Box get_original_logical_bounding_box() const
                {return get_original_true_bounding_box();}
            pga2::Bivector get_center() const;
            pga2::Bivector get_left() const;
            pga2::Bivector get_right() const;
            pga2::Bivector get_up() const;
            pga2::Bivector get_down() const;
            pga2::Bivector get_upper_left() const;
            pga2::Bivector get_upper_right() const;
            pga2::Bivector get_lower_left() const;
            pga2::Bivector get_lower_right() const;

            Object& next_to(
                const pga3::Trivector& point,
                const pga3::Trivector& direction,
                double buff = 0.25
            );
            Object& next_to(
                Object& object,
                const pga3::Trivector& direction,
                double buff = 0.25
            );
            Object& next_to(
                const pointlike auto& point,
                const pointlike auto& direction,
                double buff = 0.25
            )
            {
                return next_to(
                    pointlike_to_pga3(point),
                    pointlike_to_pga3(direction),
                    buff
                );
            }
            Object& next_to(
                Object& object,
                const pointlike auto& direction,
                double buff = 0.25
            )
            {
                return next_to(object, pointlike_to_pga3(direction), buff);
            }

            GANIM_TRANSFORMABLE_CHAIN_DECLS(Object);

        private:

            Color M_color = {255, 255, 255, 255};
            double M_scale = 1;
            double M_draw_fraction = 1;
            bool M_visible = false;
            bool M_creating = false;
            double M_noise_creating = 0.0;
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
    Type& scale(const pointlike auto& about_point, double amount) \
        {Object::scale(about_point, amount); return *this;} \

#endif
