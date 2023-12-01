#ifndef GANIM_GL_BUFFER_HPP
#define GANIM_GL_BUFFER_HPP

/** @file
 * @brief Contains the @ref ganim::gl::Buffer "Buffer" class
 */

namespace ganim::gl {
    /** @brief A thin RAII wrapper around OpenGL buffers.
     *
     * The constructor calls `glGenBuffer` while the destructor calls
     * `glDeleteBuffer`.  This type is implicitly convertible to an unsigned
     * integer, which is the buffer id, so you can pass this type to OpenGL
     * functions that expect a buffer id.
     */
    class Buffer {
        public:
            Buffer();
            ~Buffer();
            Buffer(unsigned id) {M_id = id;}
            Buffer(Buffer&&) noexcept;
            Buffer(const Buffer&)=delete;
            Buffer& operator=(Buffer&&) noexcept;
            Buffer& operator=(const Buffer&)=delete;
            operator unsigned() const {return M_id;}

        private:
            unsigned M_id = 0;
    };
}

#endif
