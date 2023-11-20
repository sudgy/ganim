R"(
#version 330 core
in vec4 out_color;
out vec4 color;

uniform vec4 object_color;

void main()
{
    color = out_color * object_color;
}
)"
