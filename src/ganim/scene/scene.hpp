#ifndef GANIM_SCENE_SCENE_HPP
#define GANIM_SCENE_SCENE_HPP

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
             * @param coord_width The width, in coordinate units, of this scene.
             * @param coord_height The height, in coordinate units, of this
             * scene.
             * @param fps The framerate of this scene.
             */
            Scene(
                std::string filename,
                int pixel_width,
                int pixel_height,
                double coord_width,
                double coord_height,
                int fps
            );
            /** @brief Write the current frame to a file
             *
             * If the scene hasn't rendered anything yet, it will advance one
             * frame first.  It will always make a .png file.
             */
            void write_to_image(std::string filename);

        private:
            virtual void process_frame() override;

            VideoWriter M_writer;
            std::unique_ptr<std::uint8_t[]> M_data;
            bool M_processed = false;
    };
}

#endif
