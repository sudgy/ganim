#ifndef GANIM_GL_VERTEX_ARRAY_HPP
#define GANIM_GL_VERTEX_ARRAY_HPP

/** @file
 * @brief Contains the @ref ganim::gl::VertexArray "VertexArray" class
 */

namespace ganim::gl {
    /** @brief A thin RAII wrapper around OpenGL vertex arrays.
     *
     * The constructor calls `glGenVertexArrays` while the destructor calls
     * `glDeleteVertexArrays`.  This type is implicitly convertible to an
     * unsigned integer, which is the vertex array id, so you can pass this type
     * to OpenGL functions that expect a vertex array id.
     */
    class VertexArray {
        public:
            VertexArray();
            ~VertexArray();
            VertexArray(unsigned id) {M_id = id;}
            VertexArray(VertexArray&&) noexcept;
            VertexArray(const VertexArray&)=delete;
            VertexArray& operator=(VertexArray&&) noexcept;
            VertexArray& operator=(const VertexArray&)=delete;
            operator unsigned() const {return M_id;}

        private:
            unsigned M_id {0};
    };
}

#endif
