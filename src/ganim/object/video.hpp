#ifndef GANIM_OBJECT_VIDEO_HPP
#define GANIM_OBJECT_VIDEO_HPP

#include "texture_shape.hpp"

namespace ganim {
    class Video : public TextureShape<Shape> {
        public:
            explicit Video(const std::string& filename);
            ~Video();
            Video(const Video&)=delete;
            Video& operator=(const Video&)=delete;
            Video(Video&&) noexcept;
            Video& operator=(Video&&) noexcept;

        private:
            void update();
            bool send_packet();

            struct Impl;
            std::unique_ptr<Impl> M_impl;
    };

    inline ObjectPtr<Video> make_video(const std::string& filename)
    {
        return ObjectPtr<Video>(filename);
    }
}

#endif
