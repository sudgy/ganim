#ifndef GANIM_VIDEO_WRITER_HPP
#define GANIM_VIDEO_WRITER_HPP

/** @file
 * @brief The @ref ganim::VideoWriter "VideoWriter" class.
 */

#include <memory>
#include <span>
#include <cstdint>

namespace ganim {
    /** @brief The class that actually writes the images to a video file.
     *
     * To use it just construct it, pass frames to it whenever you want using
     * @ref write_frame, and then to finish writing to the file call @ref finish
     * (which is also called by the destructor, so this last step is not always
     * necessary).
     */
    class VideoWriter {
        public:
            /** @brief Constructor.
             * @param filename The name of the file you want to create,
             * including the extension.
             * @param width The width of the output video.
             * @param height The height of the output video.
             * @param fps The framerate of the output video.
             */
            VideoWriter(std::string filename, int width, int height, int fps);
            /** @brief Destructor.
             *
             * This calls @ref finish if it hasn't been called already.
             */
            ~VideoWriter();
            VideoWriter(const VideoWriter&)=delete;
            VideoWriter& operator=(const VideoWriter&)=delete;
            VideoWriter(VideoWriter&&);
            VideoWriter& operator=(VideoWriter&&);

            /** @brief Write a frame to the file.
             *
             * @param image The data for a frame.  It must be RGB data, not RGBA
             * data.  Thus, `image.size()` must be equal to
             * `width * height * 3`.
             * @throw std::invalid_argument When
             * `image.size() != width * height * 3`.
             */
            void write_frame(std::span<std::uint8_t> image);
            /** @brief Finish writing to the file and close the file.
             *
             * This is called automatically by the destructor, so only call it
             * if you want the file to be finished before the destructor would
             * get called.
             */
            void finish();

        private:
            class Impl;
            std::unique_ptr<Impl> M_impl;
            void receive_packets();
    };
}

#endif
