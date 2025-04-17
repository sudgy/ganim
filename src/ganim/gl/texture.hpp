#ifndef GANIM_GL_TEXTURE_HPP
#define GANIM_GL_TEXTURE_HPP

/** @file
 * @brief Contains the @ref ganim::gl::Texture "Texture" class
 */

#include <cstdint>
#include <string>

namespace ganim::gl {
    /** @brief An RAII wrapper around OpenGL textures.
     *
     * The constructor calls `glGenTextures` while the destructor calls
     * `glDeleteTextures`.  This type is implicitly convertible to an unsigned
     * integer, which is the texture id, so you can pass this type to OpenGL
     * functions that expect a texture id.  It also includes several ways to
     * construct textures from existing images.
     */
    class Texture {
        public:
            Texture();
            ~Texture();
            /** @brief Creates a texture from an in-memory image
             *
             * @param data The image data.  It should be an array of RGBA pixel
             * values with `width * height` pixels, so the total size of the
             * array should be `4 * width * height`.  The first pixel is the
             * bottom left of the image, and then it advances in the x
             * direction, then in the y direction.
             * @param width The width of the image, in pixels
             * @param height The height of the image, in pixels
             */
            Texture(std::uint8_t* data, int width, int height);
            Texture(unsigned id) {M_id = id;}
            Texture(Texture&&) noexcept;
            Texture(const Texture&)=delete;
            Texture& operator=(Texture&&) noexcept;
            Texture& operator=(const Texture&)=delete;
            operator unsigned() const {return M_id;}

            void write_to_file(std::string filename);

        private:
            unsigned M_id = 0;
    };
}

#endif
