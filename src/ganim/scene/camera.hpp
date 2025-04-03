#ifndef GANIM_SCENE_CAMERA_HPP
#define GANIM_SCENE_CAMERA_HPP

/** @file
 * @brief Contains the @ref ganim::Camera "Camera" class
 */

#include "ganim/object/bases/transformable.hpp"

namespace ganim {

/** @brief The camera class
 *
 * It derives from @ref Transformable, and the rotor represents the position and
 * orientation of the camera, where its starting position and orientation are at
 * the origin pointing in the negative z direction.  Note that when using the
 * camera's rotor, it's usually better to move everything else, not the camera,
 * meaning that you need to transform with the reverse of the rotor, not the
 * rotor itself.
 *
 * In addition to being transformable, it also keeps track of information needed
 * to display with OpenGL's coordinates.  OpenGL assumes that coordinates on the
 * screen go from -1 to +1.  To use this class correctly to display on the
 * screen, you must do the following:
 *  - Apply the reverse rotor to your object.  The camera is now at the origin
 *    facing downwards.
 *  - Join the origin with the point you are projecting, and find the
 *    intersection with the plane e3 + e0 (i.e. the xy-plane shifted down by one
 *    in the z direction).
 *  - Take the resulting point and multiply the x component with the result of
 *    @ref get_x_scale and multiply the y component with the result of @ref
 *    get_y_scale.
 *
 *  Doing these things will project your points onto the [-1, 1] coordinates
 *  that OpenGL expects.
 */
class Camera : public Transformable {
    public:
        /** @brief Constructor
         *
         * @param distance The distance of the camera from the origin.  It
         * defaults to going in the positive z direction by this amount.  If you
         * want to move the camera to a different point, use the @ref
         * Transformable functions.  This distance is used to calculate the
         * field of view, so it's suggested that you set it to a reasonable
         * value (the default is 20) even if you don't plan on having the camera
         * at that distance from the origin.
         * @param width The width of what the camera will see.
         * @param height The height of what the camera will see.
         */
        Camera(double distance, double width, double height);
        /** @brief Get the value to scale the x coordinate by after projecting
         *
         * See the description of the class for more details.
         */
        double get_x_scale() const {return M_x_scale;}
        /** @brief Get the value to scale the y coordinate by after projecting
         *
         * See the description of the class for more details.
         */
        double get_y_scale() const {return M_y_scale;}
        /** @brief Get the starting width passed to the constructor
         */
        double get_starting_width() const {return M_starting_width;}
        /** @brief Get the starting width passed to the constructor
         */
        double get_starting_height() const {return M_starting_height;}
        /** @brief Reset the camera back to its starting position */
        virtual Camera& reset() override;

    private:
        double M_x_scale = -1;
        double M_y_scale = -1;
        double M_starting_width = -1;
        double M_starting_height = -1;
        pga3::Even M_original_rotor;
};

}

#endif
