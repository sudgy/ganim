R"(
#version 330 core
in float vertex_t;
in vec4 out_color;
in vec2 out_tex_coord;
out vec4 color;

uniform vec4 object_color;
uniform float this_t;
uniform sampler2D in_texture;

void main()
{
    color = out_color * object_color * texture(in_texture, out_tex_coord);
    if (this_t < vertex_t) discard;
}
)"
