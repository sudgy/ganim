#ifndef GANIM_SCENE_BASE_H
#define GANIM_SCENE_BASE_H

/** @file
 * @brief The @ref ganim::SceneBase "SceneBase" class
 */

#include "ganim/gl/framebuffer.hpp"
#include "ganim/gl/texture.hpp"

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
             */
            SceneBase(
                int pixel_width,
                int pixel_height
            );
            virtual ~SceneBase()=default;
            /** @brief Process the scene for one frame.
             *
             * This will update all objects, draw them all onto the scene's
             * internal framebuffer, and then call @ref process_frame.
             */
            void frame_advance();

            /** @brief Get the width, in pixels, of this scene. */
            constexpr int pixel_width() const {return M_pixel_width;}
            /** @brief Get the height, in pixels, of this scene. */
            constexpr int pixel_height() const {return M_pixel_height;}

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
    };
}

#endif
