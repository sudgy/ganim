#ifndef GANIM_OBJECT_SHADERS_HPP
#define GANIM_OBJECT_SHADERS_HPP

/** @file
 * @brief Contains global references to all of the shaders used in ganim.
 *
 * I'm still not sure if this is the best way to use shaders, but it's what I'm
 * going with for now.
 */

#include "ganim/gl/shader.hpp"

namespace ganim {
    struct ShaderParts {
        std::vector<const char*> vertex_inputs;
        std::vector<const char*> vertex_outputs;
        const char* vertex_uniforms = nullptr;
        const char* vertex_functions = nullptr;
        const char* vertex_main = nullptr;
        const char* fragment_uniforms = nullptr;
        const char* fragment_functions = nullptr;
        const char* fragment_main = nullptr;
    };
    ShaderParts& basic_shader_parts();
    ShaderParts& texture_shader_parts();
    ShaderParts& outline_shader_parts();
    ShaderParts& create_shader_parts();
    ShaderParts& noise_create_shader_parts();
    gl::Shader& get_shader(const std::vector<ShaderParts*>& parts);
}

#endif
