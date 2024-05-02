#ifndef GANIM_UTIL_DISTANCE_TRANSFORM_HPP
#define GANIM_UTIL_DISTANCE_TRANSFORM_HPP

#include "ganim/gl/texture.hpp"

namespace ganim {
    /** @brief Computes the distance transform of a given thresholded texture
     *
     * This will take in a given texture in GL_R8UI format and produce a new
     * texture in GL_R32F format.  It will calculate the distance transform of
     * the input texture.  The value in each pixel will be the distance from
     * that pixel to the closest pixel with a value equal to 255 in the input
     * texture.
     *
     * For now, this only supports square textures of side lengths that are a
     * power of two.  If they are not, an `std::invalid_argument` will be
     * thrown.
     *
     * @param tex The texture.  Its format must be GL_R8UI.
     * @param size The width and height of the texture, in pixels.
     *
     * @return The distance transform.  Its format will be GL_R32F.
     */
    gl::Texture distance_transform(
        const gl::Texture& input,
        int size
    );
}

#endif
