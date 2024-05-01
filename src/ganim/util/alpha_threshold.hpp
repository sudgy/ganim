#ifndef GANIM_UTIL_ALPHA_THRESHOLD_HPP
#define GANIM_UTIL_ALPHA_THRESHOLD_HPP

#include "ganim/gl/texture.hpp"

namespace ganim {
    /** @brief Computes an "alpha threshold" on a given texture
     *
     * This will take in a given texture in GL_RGBA32F format and produces a new
     * texture in GL_R8UI format.  Any pixel with alpha value greater than the
     * `threshold` value will be 255 in the resulting texture, and all other
     * pixels will be 0.
     *
     * For now, width and height must be a multiple of 8.  If they are not, an
     * `std::invalid_argument` will be thrown.
     *
     * @param tex The texture.  Its format must be GL_RGBA32F.
     * @param threshold The alpha threshold value to use.  Anything greater than
     * or equal to it will be 255 in the output image, and everything else will
     * be 0.
     * @param width The width of the texture, in pixels.
     * @param height The height of the texture, in pixels.
     *
     * @return The thresholded texture.  Its format will be GL_R8UI.
     */
    gl::Texture alpha_threshold(
        const gl::Texture& tex,
        float threshold,
        int width,
        int height
    );
}

#endif
