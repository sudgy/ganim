R"(
#version 330 core
in float vertex_t;
in vec4 out_color;
out vec4 color;

uniform vec4 object_color;
uniform float this_t;

void main()
{
    color = out_color * object_color;
    if (this_t < vertex_t) discard;
}
)"
