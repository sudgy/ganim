#ifndef GANIM_GL_SHADER_HPP
#define GANIM_GL_SHADER_HPP

/** @file
 * @brief Contains the @ref ganim::gl::Shader "Shader" class
 */

#include <vector>

#include "ganim/ga/pga3.hpp"

namespace ganim::gl {
    /** @brief An RAII wrapper around OpenGL shaders.
     *
     * Unlike the other OpenGL wrappers, this one actually does a bit more.  To
     * construct it, you need to pass two instances of @ref Shader::Source that
     * represent the vertex shader and the fragment shader.  Then it will do all
     * of the compiling and linking itself.  This type is implicitly convertible
     * to an unsigned integer, which is the shader program id, so you can pass
     * this type to OpenGL functions that expect a shader id.  You can also get
     * uniform indices with @ref get_uniform.
     */
    class Shader {
        public:
            /** @brief Represents the source of an OpenGL shader
             *
             * In the end it's just a wrapper around a vector of `const char*`s.
             * You can add to it with @ref add_source, and you can get all of
             * the sources with @ref source.
             */
            class Source {
                public:
                    /** @brief Creates an empty shader source. */
                    Source()=default;
                    /** @brief Add a piece of code to the source. */
                    void add_source(const char* source);
                    /** @brief Get the source. */
                    const std::vector<const char*>& source() const;

                private:
                    std::vector<const char*> M_source;
            };
            /** @brief Create a shader with corresponding vertex and fragment
             * sources.
             *
             * Note that at least currently, geometry shaders are not possible.
             */
            Shader(const Source& vertex, const Source& fragment);
            /** @brief Create a compute shader
             */
            Shader(const Source& compute);
            ~Shader();
            Shader(Shader&&) noexcept;
            Shader(const Shader&)=delete;
            Shader& operator=(Shader&&) noexcept;
            Shader& operator=(const Shader&)=delete;
            operator unsigned() const {return M_program_id;}
            /** @brief Get the id of a uniform.
             *
             * This is equivalent to `glGetUniformLocation(*this, name)`, but
             * there's also some error checking.
             */
            int get_uniform(const char* name) const;
            void set_rotor_uniform(const char* name, const pga3::Even& rotor);

        private:
            unsigned M_program_id = 0;
    };
}

#endif
