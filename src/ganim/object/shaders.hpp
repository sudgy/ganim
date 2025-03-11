#ifndef GANIM_OBJECT_SHADERS_HPP
#define GANIM_OBJECT_SHADERS_HPP

#include "ganim/gl/shader.hpp"

namespace ganim {
    /** @brief A bitmask enum type that can represent various features that you
     * want in shaders
     */
    enum class ShaderFeature : std::uint64_t {
        Time = 1 << 0,
        VertexColors = 1 << 1,
        Texture = 1 << 2,
        Vector = 1 << 3,
        Create = 1 << 4,
        NoiseCreate = 1 << 5,
        FaceShading = 1 << 6,
        DepthPeeling = 1 << 7,
        Dash = 1 << 8,
        TextureTransform = 1 << 9,
        Outline = 1 << 10
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


    /** @brief Get a shader with particular features
     *
     * To use this, make a set of features using @ref ShaderFeature using
     * bitwise operations, then call this function with it.  The returned shader
     * will have all of those features.
     */
    gl::Shader& get_shader(ShaderFeature features);
}

#endif
