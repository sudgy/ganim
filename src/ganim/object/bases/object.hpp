#ifndef GANIM_OBJECT_OBJECT_HPP
#define GANIM_OBJECT_OBJECT_HPP

/** @file
 * @brief Contains the @ref ganim::Object "Object" class
 */

#include "transformable.hpp"

#include "ganim/color.hpp"
#include "ganim/util/box.hpp"
#include "ganim/scene/camera.hpp"

#include "ganim/gl/texture.hpp"

namespace ganim {
    /** @brief The base class for objects that can be drawn and have some kind
     * of extent
     *
     * Most of the functions in this class return `*this` to allow for chaining.
     * If you want the chaining to work on subclasses, use the @ref
     * GANIM_OBJECT_CHAIN_DECLS macro.
     */
    class Object : public Transformable, public BoxInterface {
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
             * @param shift_depth Whether or not to make the depth buffer values
             * of the outline be further away than the object itself.  This is
             * necessary sometimes in 3D to prevent the outline being drawn over
             * the object due to floating point inaccuracies.  It should be
             * false in 2D scenes though where which objects cover others is
             * done by drawing order rather than the depth buffer.
             */
            virtual void set_outline(
                const Color& color,
                double thickness,
                bool shift_depth = false
            )=0;
            /** @brief Set the outline to be invalid
             *
             * The usual outline algorithm is expensive, so it is only ever done
             * once when the object is first drawn.  Call this function when the
             * outline needs to be calculated again, such as when the shape of
             * the object has changed.
             */
            virtual void invalidate_outline()=0;
            /** @brief Get the outline color. */
            virtual Color get_outline_color() const=0;
            /** @brief Get the outline thickness. */
            virtual double get_outline_thickness() const=0;

            /** @brief Used internally for order-independent transparency. */
            virtual void set_peeling_depth_buffer(gl::Texture* texture)
                {M_peeling_depth_buffer = texture;}
            /** @brief Used internally for order-independent transparency. */
            gl::Texture* peeling_depth_buffer() const
                {return M_peeling_depth_buffer;}

            /** @brief Set the color of this object. */
            virtual Object& set_color(Color color);
            /** @brief Get the color of this object. */
            Color get_color() const {return M_color.back();}
            /** @brief Push the current color onto the color stack
             *
             * The color on the top of the color stack is the one that is used
             * in get_color and set_color.  To undo this operation, call @ref
             * pop_color.
             */
            virtual Object& push_color();
            /** @brief Push a color onto the color stack
             *
             * The color on the top of the color stack is the one that is used
             * in get_color and set_color.  To undo this operation, call @ref
             * pop_color.
             */
            virtual Object& push_color(Color color);
            /** @brief Pop a color from the color stack
             *
             * This will remove the current color and go back to the color from
             * before the last call to @ref push_color.  If there is only one
             * color in the color stack, this function does nothing.
             */
            virtual Object& pop_color();
            /** @brief Set the opacity of this object
             *
             * @param opacity The opacity to change to, from 0 to 1.  0 is fully
             * transparent, while 1 is fully opaque.
             */
            virtual Object& set_opacity(double opacity);
            /** @brief Get the opacity of this object
             *
             * @param opacity The opacity from 0 to 1.  0 is fully transparent,
             * while 1 is fully opaque.
             */
            double get_opacity() const {return M_opacity;}

            /** @brief Scale the object about its origin */
            Object& scale(double amount);
            /** @brief Scale the object about a particular point */
            Object& scale(double amount, const pointlike auto& p)
                {return scale(amount, pointlike_to_pga3(p));}
            /** @brief Scale the object about a particular point */
            virtual Object& scale(
                double amount,
                const pga3::Trivec& about_point
            );
            /** @brief Get the amount that this object is scaled */
            double get_scale() const;
            /** @brief Reset the scale back to 1.0
             *
             * This is mainly here if, for any reason, an object's scale was set
             * to zero.  Note that any shifting caused by previous scaling
             * around non-origin points is *not* reset.
             */
            void reset_scale() {M_scale = 1.0;}
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

            /** @brief Set what value to add to the z value when writing to the
             * depth buffer
             *
             * This is mainly used for overlapping objects in 2D scenes.  The
             * depth z value will be added to the z value when writing to the
             * depth buffer, but it will not be used when positioning the object
             * in 3D space.
             */
            virtual Object& set_depth_z(double depth_z)
                {M_depth_z = depth_z; return *this;}
            /** @brief Get what value to add to the z value when writing to the
             * depth buffer
             *
             * This is mainly used for overlapping objects in 2D scenes.  The
             * depth z value will be added to the z value when writing to the
             * depth buffer, but it will not be used when positioning the object
             * in 3D space.
             */
            double get_depth_z() const {return M_depth_z;}

            /** @brief Interpolate between two Objects */
            virtual void interpolate(
                const Animatable& start,
                const Animatable& end,
                double t
            ) override;
            ObjectPtr<Object> polymorphic_copy() const;

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
            /** @brief Set whether or not this object is fixed in frame, i.e. is
             * not affected by the motion of the camera.
             */
            virtual void set_fixed_in_frame(bool fixed_in_frame)
                {M_fixed_in_frame = fixed_in_frame;}
            /** @brief See whether or not this object is fixed in frame, i.e. is
             * not affected by the motion of the camera.
             */
            bool is_fixed_in_frame() const {return M_fixed_in_frame;}
            /** @brief Set whether or not this object has a fixed orientation,
             * i.e. is always facing the camera no matter what position it's at
             * in 3D space.
             */
            virtual void set_fixed_orientation(bool fixed_orientation)
                {M_fixed_orientation = fixed_orientation;}
            /** @brief See whether or not this object has a fixed orientation,
             * i.e. is always facing the camera no matter what position it's at
             * in 3D space.
             */
            bool is_fixed_orientation() const {return M_fixed_orientation;}
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

            /** @brief Set the x coordinate of the center of this object. */
            void set_x(double x);
            /** @brief Set the y coordinate of the center of this object. */
            void set_y(double y);
            /** @brief Set the z coordinate of the center of this object. */
            void set_z(double z);
            /** @brief Get the x coordinate of the center of this object. */
            double get_x() const;
            /** @brief Get the y coordinate of the center of this object. */
            double get_y() const;
            /** @brief Get the z coordinate of the center of this object. */
            double get_z() const;

            /** @brief Place the object next to another point in a certain
             * direction.
             *
             * @param point The point to be next to.
             * @param direction the direction relative to the point to place the
             * object.
             * @param buff The distance between the point and the object.
             */
            Object& next_to(
                const pga3::Trivec& point,
                const pga3::Trivec& direction,
                double buff = 0.25
            );
            /** @brief Place the object next to another object in a certain
             * direction.
             *
             * @param object The object to be next to.
             * @param direction the direction relative to the target object to
             * place this object.
             * @param buff The distance between the bounding boxes of the two
             * objects.
             */
            Object& next_to(
                const Object& object,
                const pga3::Trivec& direction,
                double buff = 0.25
            );
            /** @brief Place the object next to another point in a certain
             * direction.
             *
             * @param point The point to be next to.
             * @param direction the direction relative to the point to place the
             * object.
             * @param buff The distance between the point and the object.
             */
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
            /** @brief Place the object next to another object in a certain
             * direction.
             *
             * @param object The object to be next to.
             * @param direction the direction relative to the target object to
             * place this object.
             * @param buff The distance between the bounding boxes of the two
             * objects.
             */
            Object& next_to(
                const Object& object,
                const pointlike auto& direction,
                double buff = 0.25
            )
            {
                return next_to(object, pointlike_to_pga3(direction), buff);
            }

            /** @brief Align an object to a particular point
             *
             * @param point The point to align to
             * @param direction The side of the object to be aligned to the
             * point
             */
            Object& align_to(
                const pga3::Trivec& point,
                const pga3::Trivec& direction
            );
            /** @brief Align an object to another object
             *
             * @param object The object to align to
             * @param direction The side of the objects to be aligned
             */
            Object& align_to(
                const Object& object,
                const pga3::Trivec& direction
            );
            /** @brief Align an object to a particular point
             *
             * @param point The point to align to
             * @param direction The side of the object to be aligned to the
             * point
             */
            Object& align_to(
                const pointlike auto& point,
                const pointlike auto& direction
            )
            {
                return align_to(
                    pointlike_to_pga3(point),
                    pointlike_to_pga3(direction)
                );
            }
            /** @brief Align an object to another object
             *
             * @param object The object to align to
             * @param direction The side of the objects to be aligned
             */
            Object& align_to(
                const Object& object,
                const pointlike auto& direction
            )
            {
                return align_to(object, pointlike_to_pga3(direction));
            }
            /** @brief Align an object to a particular object in two directions
             *
             * Aligning with a direction like e1 + e2 will not do what you might
             * expect, so call this function instead, which will align along two
             * different directions simultaneously.  Equivalent to calling align
             * with each direction separately.
             *
             * @param object The object to align to
             * @param direction1 The first direction to align to
             * @param direction2 The second direction to align to
             */
            Object& align_to(
                const Object& object,
                const pointlike auto& direction1,
                const pointlike auto& direction2
            )
            {
                align_to(object, direction1);
                return align_to(object, direction2);
            }
            /** @brief Align an object to a particular point in two directions
             *
             * Aligning with a direction like e1 + e2 will not do what you might
             * expect, so call this function instead, which will align along two
             * different directions simultaneously.  Equivalent to calling align
             * with each direction separately.
             *
             * @param point The point to align to
             * @param direction1 The first direction to align to
             * @param direction2 The second direction to align to
             */
            Object& align_to(
                const pointlike auto& point,
                const pointlike auto& direction1,
                const pointlike auto& direction2
            )
            {
                align_to(point, direction1);
                return align_to(point, direction2);
            }
            /** @brief Move an object to the edge of the screen.
             *
             * Since what defines "the screen" is given by the camera, you must
             * pass a camera to this function as well.
             *
             * @param camera The camera that will define the screen.  You should
             * probably just pull this directly from the scene.
             * @param direction the side of the screen to go to
             * @param buff The distance between the actual edge of the screen
             * and the edge of the object.
             */
            Object& to_edge(
                const Camera& camera,
                const pga3::Trivec& direction,
                double buff = 0.5
            );
            /** @brief Move an object to the edge of the screen.
             *
             * Since what defines "the screen" is given by the camera, you must
             * pass a camera to this function as well.
             *
             * @param camera The camera that will define the screen.  You should
             * probably just pull this directly from the scene.
             * @param direction the side of the screen to go to
             * @param buff The distance between the actual edge of the screen
             * and the edge of the object.
             */
            Object& to_edge(
                const Camera& camera,
                const pointlike auto& direction,
                double buff = 0.5
            )
            {
                return to_edge(camera, pointlike_to_pga3(direction), buff);
            }

            /** @brief Get an object visually representing the object's bounding
             * box for debug purposes.
             *
             * @param color The color to make the resulting object be.  It
             * defaults to a transparent gray.  You probably want transparent
             * colors in general to make debugging easier.
             */
            ObjectPtr<Object>
                get_bounding_box_object(Color color = "80808080") const;

            GANIM_TRANSFORMABLE_CHAIN_DECLS(Object);

        private:
            virtual Object* polymorphic_copy_impl() const override;
            virtual std::pair<pga3::Trivec, pga3::Trivec> get_box()
                const override final;

            double M_scale = 1;
            double M_draw_fraction = 1;
            double M_noise_creating = 0.0;
            double M_depth_z = 0.0;
            std::vector<Color> M_color = {{255, 255, 255, 255}};
            double M_opacity = 1;
            bool M_visible = false;
            bool M_creating = false;
            bool M_fixed_in_frame = false;
            bool M_fixed_orientation = false;
            gl::Texture* M_peeling_depth_buffer = nullptr;
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
    Type& set_opacity(double opacity) \
        {Object::set_opacity(opacity); return *this;} \
    Type& scale(double amount) \
        {Object::scale(amount); return *this;} \
    Type& scale(double amount, const pointlike auto& about_point) \
        {Object::scale(amount, about_point); return *this;} \

#endif
