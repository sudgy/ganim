#ifndef GANIM_GL_TEXTURE_HPP
#define GANIM_GL_TEXTURE_HPP

/** @file
 * @brief Contains the @ref ganim::gl::Texture "Texture" class
 */

namespace ganim::gl {
    /** @brief A thin RAII wrapper around OpenGL textures.
     *
     * The constructor calls `glGenTextures` while the destructor calls
     * `glDeleteTextures`.  This type is implicitly convertible to an unsigned
     * integer, which is the texture id, so you can pass this type to OpenGL
     * functions that expect a texture id.
     */
    class Texture {
        public:
            Texture();
            ~Texture();
            Texture(Texture&&) noexcept;
            Texture(const Texture&)=delete;
            Texture& operator=(Texture&&) noexcept;
            Texture& operator=(const Texture&)=delete;
            operator unsigned() const {return M_id;}

        private:
            unsigned M_id = 0;
    };
}

#endif
