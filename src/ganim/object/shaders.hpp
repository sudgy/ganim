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
    /** @brief Get the shader used for @ref Shape "Shape"s. */
    gl::Shader& shape_shader();
}

#endif
