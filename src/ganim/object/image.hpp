#ifndef GANIM_OBJECT_IMAGE_HPP
#define GANIM_OBJECT_IMAGE_HPP

#include "texture_shape.hpp"

#include "ganim/gl/texture.hpp"

namespace ganim {
    class Image : public TextureShape<Shape> {
        public:
            explicit Image(const std::string& filename);

        private:
            gl::Texture M_texture;
    };

    inline ObjectPtr<Image> make_image(const std::string& filename)
    {
        return ObjectPtr<Image>(filename);
    }
}

#endif
