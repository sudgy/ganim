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
    enum class ShaderFeature : std::uint64_t {
        Time = 1 << 0,
        VertexColors = 1 << 1,
        Texture = 1 << 2,
        Vector = 1 << 3,
        Create = 1 << 4,
        NoiseCreate = 1 << 5,
        FaceShading = 1 << 6
    };
    constexpr bool operator&(ShaderFeature f1, ShaderFeature f2)
    {
        return static_cast<bool>(
            static_cast<std::uint64_t>(f1) & static_cast<std::uint64_t>(f2));
    }
    constexpr ShaderFeature operator|(ShaderFeature f1, ShaderFeature f2)
    {
        return static_cast<ShaderFeature>(
            static_cast<std::uint64_t>(f1) | static_cast<std::uint64_t>(f2));
    }
    constexpr ShaderFeature operator^(ShaderFeature f1, ShaderFeature f2)
    {
        return static_cast<ShaderFeature>(
            static_cast<std::uint64_t>(f1) ^ static_cast<std::uint64_t>(f2));
    }
    constexpr ShaderFeature& operator|=(ShaderFeature& f1, ShaderFeature f2)
        {f1 = f1 | f2; return f1;}
    constexpr ShaderFeature& operator^=(ShaderFeature& f1, ShaderFeature f2)
        {f1 = f1 ^ f2; return f1;}


    gl::Shader& get_shader(ShaderFeature features);
}

#endif
