#include "shaders.hpp"

#include <unordered_map>
#include <sstream>

using namespace ganim;

namespace {
    auto basic_shader_parts_ = ShaderParts(
        {"vec3 in_pos;\n", "float in_t;\n", "vec4 in_color;\n"},
        {"float vertex_t;\n", "vec4 out_color;\n"},
        #include "ganim/shaders/basic_shader_vertex_uniforms.glsl"
        ,
        #include "ganim/shaders/basic_shader_vertex_functions.glsl"
        ,
        #include "ganim/shaders/basic_shader_vertex_main.glsl"
        #include "ganim/shaders/basic_shader_color_vertex_main.glsl"
        ,
        #include "ganim/shaders/basic_shader_fragment_uniforms.glsl"
        ,
        nullptr,
        #include "ganim/shaders/basic_shader_fragment_main.glsl"
        #include "ganim/shaders/basic_shader_color_fragment_main.glsl"
    );
    auto texture_shader_parts_ = ShaderParts(
        {"vec2 in_tex_coord;\n"},
        {"vec2 out_tex_coord;\n"},
        nullptr,
        nullptr,
        #include "ganim/shaders/texture_shader_vertex_main.glsl"
        ,
        #include "ganim/shaders/texture_shader_fragment_uniforms.glsl"
        ,
        nullptr,
        #include "ganim/shaders/texture_shader_fragment_main.glsl"
    );
    auto create_shader_parts_ = ShaderParts(
        {},
        {},
        nullptr,
        nullptr,
        nullptr,
        #include "ganim/shaders/create_shader_fragment_uniforms.glsl"
        ,
        nullptr,
        #include "ganim/shaders/create_shader_fragment_main.glsl"
    );
    auto noise_create_shader_parts_ = ShaderParts(
        {},
        {"vec2 noise_coord;\n"},
        nullptr,
        nullptr,
        #include "ganim/shaders/noise_create_shader_vertex_main.glsl"
        ,
        #include "ganim/shaders/noise_create_shader_fragment_uniforms.glsl"
        ,
        #include "ganim/shaders/noise_create_shader_fragment_functions.glsl"
        ,
        #include "ganim/shaders/noise_create_shader_fragment_main.glsl"
    );
    auto vector_shader_parts_ = ShaderParts(
        {"vec3 real_in_pos;\n", "float in_t;\n"},
        {"float vertex_t;\n"},
        #include "ganim/shaders/basic_shader_vertex_uniforms.glsl"
        #include "ganim/shaders/vector_shader_vertex_uniforms.glsl"
        ,
        #include "ganim/shaders/basic_shader_vertex_functions.glsl"
        ,
        #include "ganim/shaders/vector_shader_vertex_main.glsl"
        #include "ganim/shaders/basic_shader_vertex_main.glsl"
        ,
        #include "ganim/shaders/basic_shader_fragment_uniforms.glsl"
        ,
        nullptr,
        #include "ganim/shaders/basic_shader_fragment_main.glsl"
    );

    struct vector_hash {
        std::size_t operator()(const std::vector<ShaderParts*>& parts) const
        {
            const auto data = reinterpret_cast<const char*>(parts.data());
            const auto size = parts.size() * sizeof(ShaderParts*);
            auto str = std::string_view(data, size);
            return std::hash<std::string_view>()(str);
        }
    };

    std::unordered_map<std::vector<ShaderParts*>, gl::Shader, vector_hash>
        G_shaders;
}

namespace ganim {

ShaderParts& basic_shader_parts() {return basic_shader_parts_;}
ShaderParts& texture_shader_parts() {return texture_shader_parts_;}
ShaderParts& create_shader_parts() {return create_shader_parts_;}
ShaderParts& noise_create_shader_parts() {return noise_create_shader_parts_;}
ShaderParts& vector_shader_parts() {return vector_shader_parts_;}

gl::Shader& get_shader(const std::vector<ShaderParts*>& parts)
{
    auto it = G_shaders.find(parts);
    if (it != G_shaders.end()) return it->second;

    auto vertex = gl::Shader::Source();
    auto fragment = gl::Shader::Source();
    vertex.add_source("#version 330 core\n");
    fragment.add_source("#version 330 core\n");

    auto vertex_inputs = std::stringstream();
    auto vertex_outputs = std::stringstream();
    auto fragment_inputs = std::stringstream();
    auto i = 0;
    for (auto part : parts) {
        for (auto input : part->vertex_inputs) {
            vertex_inputs << "layout (location = " << i++ << ") in " << input;
        }
        for (auto output : part->vertex_outputs) {
            vertex_inputs << "out " << output;
            fragment_inputs << "in " << output;
        }
    }
    // For some reason, the depth buffer fails unless I set gl_FragDepth to
    // exactly what OpenGL says it's supposed to be.  I have no idea why this is
    // the case.  Since I apparently can't trust gl_FragCoord.z to be correct I
    // have to calculate the value myself in the vertex shader and pass it to
    // the fragment shader using this "confusing_z" variable.
    vertex_outputs << "out float confusing_z;\n";
    fragment_inputs << "in float confusing_z;\n";
    auto vertex_inputs_str = vertex_inputs.str();
    auto vertex_outputs_str = vertex_outputs.str();
    auto fragment_inputs_str = fragment_inputs.str();
    vertex.add_source(vertex_inputs_str.c_str());
    vertex.add_source(vertex_outputs_str.c_str());
    fragment.add_source(fragment_inputs_str.c_str());
    fragment.add_source("out vec4 color;\n");

    for (auto part : parts) {
        if (part->vertex_uniforms) vertex.add_source(part->vertex_uniforms);
        if (part->fragment_uniforms) fragment.add_source(part->fragment_uniforms);
    }
    for (auto part : parts) {
        if (part->vertex_functions) vertex.add_source(part->vertex_functions);
        if (part->fragment_functions) fragment.add_source(part->fragment_functions);
    }
    vertex.add_source("void main() {\n");
    fragment.add_source("void main() {\n");
    for (auto part : parts) {
        if (part->vertex_main) vertex.add_source(part->vertex_main);
        if (part->fragment_main) fragment.add_source(part->fragment_main);
    }
    vertex.add_source("confusing_z = 0.5*(gl_Position.z / gl_Position.w) + 0.5;\n");
    vertex.add_source("}\n");
    fragment.add_source("gl_FragDepth = confusing_z;\n");
    fragment.add_source("}\n");

    return G_shaders.emplace(parts, gl::Shader(vertex, fragment)).first->second;
}

}
