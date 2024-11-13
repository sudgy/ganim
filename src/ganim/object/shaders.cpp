#include "shaders.hpp"

#include <unordered_map>
#include <sstream>

using namespace ganim;

using enum ShaderFeature;

namespace {
    std::unordered_map<ShaderFeature, gl::Shader> G_shaders;
}

namespace ganim {

gl::Shader& get_shader(ShaderFeature features)
{
    auto it = G_shaders.find(features);
    if (it != G_shaders.end()) return it->second;

    auto vertex = gl::Shader::Source();
    auto fragment = gl::Shader::Source();
    vertex.add_source("#version 330 core\n");
    fragment.add_source("#version 330 core\n");

    if ((features & NoiseCreate) and (features & Create)) {
        features = static_cast<ShaderFeature>(features ^ Create);
    }
    if (features & Time) {
        vertex.add_source("#define TIME\n");
        fragment.add_source("#define TIME\n");
    }
    if (features & VertexColors) {
        vertex.add_source("#define VERTEX_COLORS\n");
        fragment.add_source("#define VERTEX_COLORS\n");
    }
    if (features & Texture) {
        vertex.add_source("#define TEXTURE\n");
        fragment.add_source("#define TEXTURE\n");
    }
    if (features & Vector) {
        vertex.add_source("#define VECTOR\n");
        fragment.add_source("#define VECTOR\n");
    }
    if (features & Create) {
        vertex.add_source("#define CREATE\n");
        fragment.add_source("#define CREATE\n");
    }
    if (features & NoiseCreate) {
        vertex.add_source("#define NOISE_CREATE\n");
        fragment.add_source("#define NOISE_CREATE\n");
    }

    vertex.add_source(
#include "ganim/shaders/vertex.glsl"
    );
    fragment.add_source(
#include "ganim/shaders/fragment.glsl"
    );
    return G_shaders.emplace(
        features,
        gl::Shader(vertex, fragment)
    ).first->second;
}

}
