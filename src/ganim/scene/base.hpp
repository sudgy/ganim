#ifndef GANIM_SCENE_BASE_HPP
#define GANIM_SCENE_BASE_HPP

/** @file
 * @brief The @ref ganim::SceneBase "SceneBase" class
 */

#include <vector>

#include "ganim/color.hpp"
#include "ganim/gl/framebuffer.hpp"
#include "ganim/gl/texture.hpp"

#include "ganim/object/object.hpp"

#include "camera.hpp"

namespace ganim {
    /** @brief The base class for scenes, which contains most of the scene logic
     *
     * This class has all of the scene logic except for what to actually do with
     * the final result.  Look at the subclasses to see how to actually make a
     * scene.  Note that before any version of this class is constructed, you
     * MUST have a valid and active OpenGL context!  The constructor creates a
     * framebuffer so it needs a context.
     */
    class SceneBase {
        public:
            /** @brief Constructor.
             *
             * Because this class is abstract, you probably won't be calling
             * this yourself.
             *
             * @param pixel_width The width, in pixels, of this scene.
             * @param pixel_height The height, in pixels, of this scene.
             * @param fps The framerate, in frames per second, of this scene.
             */
            SceneBase(
                int pixel_width,
                int pixel_height,
                double coord_width,
                double coord_height,
                int fps
            );
            virtual ~SceneBase()=default;
            /** @brief Process the scene for one frame.
             *
             * This will update all objects, draw them all onto the scene's
             * internal framebuffer, and then call @ref process_frame.
             */
            void frame_advance();
            /** @brief Process the scene for several frames.
             *
             * @param amount The number of frames to process.
             */
            void frame_advance(int amount);
            /** @brief Process the scene for a certain amount of time.
             *
             * @param time The time, in seconds, to process.  This time will
             * actually be converted to the nearest number of frames, so the
             * amount of time that this functions waits may not be exactly what
             * was passed into this function.
             */
            void wait(double time);

            /** @brief Get the width, in pixels, of this scene. */
            constexpr int pixel_width() const {return M_pixel_width;}
            /** @brief Get the height, in pixels, of this scene. */
            constexpr int pixel_height() const {return M_pixel_height;}
            /** @brief Set the background color of this scene. */
            constexpr void set_background_color(const Color& color)
                {M_background_color = color;}

            void add(Object& object);

        private:
            /** @brief Used for subclasses to process the frames.
             *
             * When this function is called, the scene's framebuffer will be
             * active.
             */
            virtual void process_frame()=0;

            gl::Framebuffer M_framebuffer;
            gl::Texture M_framebuffer_texture;
            int M_pixel_width = 0;
            int M_pixel_height = 0;
            int M_fps;
            Color M_background_color;
            std::vector<Object*> M_objects;
            Camera M_camera;
    };
}

#endif
