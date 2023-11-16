#ifndef GANIM_GL_FRAMEBUFFER_HPP
#define GANIM_GL_FRAMEBUFFER_HPP

/** @file
 * @brief Contains just the @ref ganim::gl::Framebuffer "Framebuffer" class
 */

namespace ganim::gl {
    /** @brief A thin RAII wrapper around OpenGL framebuffers.
     *
     * The constructor calls `glGenFramebuffers` while the destructor calls
     * `glDeleteFramebuffers`.  This type is implicitly convertible to an
     * unsigned integer, which is the framebuffer id, so you can pass this type
     * to OpenGL functions that expect a framebuffer id.
     */
    class Framebuffer {
        public:
            Framebuffer();
            ~Framebuffer();
            Framebuffer(Framebuffer&&) noexcept;
            Framebuffer(const Framebuffer&)=delete;
            Framebuffer& operator=(Framebuffer&&) noexcept;
            Framebuffer& operator=(const Framebuffer&)=delete;
            operator unsigned() const {return M_id;}

        private:
            unsigned M_id {0};
    };
}

#endif
