#ifndef GANIM_SCENE_SCENE_H
#define GANIM_SCENE_SCENE_H

/** @file
 * @brief The @ref ganim::Scene "Scene" class
 */

#include <string>

#include "base.hpp"

#include "ganim/video_writer/video_writer.hpp"

namespace ganim {
    /** @brief The scene class that writes to a file.
     *
     * Most of the logic for scenes is in @ref SceneBase.
     */
    class Scene : public SceneBase {
        public:
            /** @brief Constructor.
             *
             * @param filename The filename to write this scene to, including
             * the extension.
             * @param pixel_width The width, in pixels, of this scene.
             * @param pixel_height The height, in pixels, of this scene.
             * @param fps The framerate of this scene.
             */
            Scene(
                std::string filename,
                int pixel_width,
                int pixel_height,
                int fps
            );

        private:
            virtual void process_frame() override;

            VideoWriter M_writer;
            std::unique_ptr<std::uint8_t[]> M_data;
    };
}

#endif
