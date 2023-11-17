R"(
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
    pos.z *= pos.z / 65536;
    gl_Position = pos;
}
)"
