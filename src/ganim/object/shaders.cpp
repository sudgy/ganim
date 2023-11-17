#include "shaders.hpp"

using namespace ganim;

namespace {
    gl::Shader::Source shape_vertex()
    {
        auto source = gl::Shader::Source();
        source.add_source(R"(
#version 330 core
layout (location = 0) in vec3 in_pos;
uniform vec2 camera_scale;
void main()
{
    vec4 pos = vec4(in_pos, 1.0);
    pos.z -= 20; // This is the camera rotor but I haven't implemented it yet
    pos.w = -pos.z;
    pos.x *= camera_scale.x;
    pos.y *= -camera_scale.y; // ffmpeg has the y axis swapped
    pos.z /= 1024; // I'll make this better later
    gl_Position = pos;
})"
);
        return source;
    }
    gl::Shader::Source shape_fragment()
    {
        auto source = gl::Shader::Source();
        source.add_source(R"(
#version 330 core
out vec4 color;
void main()
{
    color = vec4(1.0, 1.0, 1.0, 1.0);
})"
);
        return source;
    }
}

namespace ganim {

gl::Shader& shape_shader()
{
    static auto result = gl::Shader(shape_vertex(), shape_fragment());
    return result;
}

}
