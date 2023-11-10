#ifndef GANIM_VIDEO_WRITER_H
#define GANIM_VIDEO_WRITER_H

#include <memory>
#include <span>
#include <cstdint>

namespace ganim {
    class VideoWriter {
        public:
            VideoWriter(std::string filename, int width, int height, int fps);
            ~VideoWriter();
            VideoWriter(const VideoWriter&)=delete;
            VideoWriter& operator=(const VideoWriter&)=delete;
            VideoWriter(VideoWriter&&);
            VideoWriter& operator=(VideoWriter&&);

            void write_frame(std::span<std::uint8_t> image);
            void finish();

        private:
            class Impl;
            std::unique_ptr<Impl> M_impl;
            void receive_packets();
    };
}

#endif
