R"(
#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in float in_t;
layout (location = 2) in vec4 in_color;
layout (location = 3) in vec2 in_tex_coord;
out float vertex_t;
out vec4 out_color;
out vec2 out_tex_coord;
uniform vec2 camera_scale;
uniform vec4 view[2];
uniform vec4 model[2];
uniform float scale;

// This calculates the sandwich product of a 3D PGA rotor r and a trivector p
// (~r * p * r).  p corresponds to the trivector (p + e0).dual(), and r
// represents a rotor under the correspondence {{a, b, c, d}, {x, y, z, w}} <->
// a + b e23 + c e31 + d e12 + x e01 + y e02 + z e03 + w e0123.
//
// This way of calculating it is due to Steven De Keninck in his upcoming paper
// "FPGA".
vec3 rotor_trivector_sandwich(vec4 r[2], vec3 p)
{
    vec3 t = cross(r[0].yzw, p) + r[1].xyz;
    return 2 * (r[0].x * t + cross(r[0].yzw, t) + r[0].yzw * r[1].w) + p;
}

// Calculates a rotor product.  The format of the rotors is the same as above.
vec4[2] rotor_mult(vec4 m[2], vec4 n[2])
{
    float a = m[0].x;
    vec3 b = m[0].yzw;
    vec3 c = m[1].xyz;
    float d = m[1].w;
    float x = n[0].x;
    vec3 y = n[0].yzw;
    vec3 z = n[1].xyz;
    float w = n[1].w;
    return vec4[2](
        vec4(a*x - dot(b, y), a*y + x*b + cross(y, b)),
        vec4(a*z - w*b + x*c - d*y + cross(z, b) + cross(y, c),
             a*w + d*x + dot(b, z) + dot(c, y))
    );
}

void main()
{
    vec4[2] r = rotor_mult(model, view);
    vec4 pos = vec4(rotor_trivector_sandwich(r, in_pos*scale), 1.0);
    pos.w = -pos.z;
    pos.x *= camera_scale.x;
    pos.y *= -camera_scale.y; // ffmpeg has the y axis swapped
    pos.z *= pos.z / 1024;
    gl_Position = pos;
    vertex_t = in_t;
    out_color = in_color;
    out_tex_coord = in_tex_coord;
}
)"
