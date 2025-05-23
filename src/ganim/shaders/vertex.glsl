R"(
layout (location = 0) in vec3 in_pos;
#ifdef TIME
layout (location = 1) in float in_t;
#endif
#ifdef VERTEX_COLORS
layout (location = 2) in vec4 in_color;
#endif
#ifdef TEXTURE
layout (location = 3) in vec2 in_tex_coord;
#endif
#ifdef TEXTURE_TRANSFORM
layout (location = 1) in vec2 in_tex_coord1;
layout (location = 2) in vec2 in_tex_coord2;
#endif
#ifdef OUTLINE
layout (location = 1) in vec2 in_tex_coord;
#endif

out VertexData {
#ifdef TIME
    float t;
#endif
#ifdef VERTEX_COLORS
    vec4 color;
#endif
#ifdef TEXTURE
    vec2 tex_coord;
#endif
#ifdef NOISE_CREATE
    vec2 noise_coord;
#endif
#ifdef FACE_SHADING
    vec3 true_position;
#endif
#ifdef TEXTURE_TRANSFORM
    vec2 out_tex_coord1;
    vec2 out_tex_coord2;
#endif
#ifdef OUTLINE
    vec2 out_tex_coord;
#endif
    vec3 window_pos;
} vs_out;

#ifdef VECTOR
uniform float mid_pos;
uniform float end_pos;
uniform float tip_size;
#endif

uniform vec2 camera_scale;
uniform vec4 view[2];
uniform vec4 model[2];
uniform float scale;
uniform float depth_z;

// Various helper functions

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
    vec3 m_in_pos = in_pos;

#ifdef VECTOR
    if (m_in_pos.x == 0.5) {
        m_in_pos.x = mid_pos;
        if (m_in_pos.y == 512 || m_in_pos.y == -512) m_in_pos.y *= tip_size/512;
    }
    else if (m_in_pos.x == 1.0) m_in_pos.x = end_pos;
#endif

    vec4[2] r = rotor_mult(model, view);
    vec4 pos = vec4(rotor_trivector_sandwich(r, m_in_pos*scale), 1.0);
#ifdef FACE_SHADING
    vs_out.true_position = pos.xyz;
#endif
    pos.w = -pos.z;
    pos.x *= camera_scale.x;
    pos.y *= -camera_scale.y; // ffmpeg has the y axis swapped
    pos.z *= (pos.z + depth_z) / 4096;
    gl_Position = pos;

#ifdef TIME
    vs_out.t = in_t;
#endif
#ifdef VERTEX_COLORS
    vs_out.color = in_color;
#endif
#ifdef NOISE_CREATE
    vs_out.noise_coord = m_in_pos.xy;
#endif
#ifdef TEXTURE
    vs_out.tex_coord = in_tex_coord;
#endif
#ifdef TEXTURE_TRANSFORM
    vs_out.out_tex_coord1 = in_tex_coord1;
    vs_out.out_tex_coord2 = in_tex_coord2;
#endif
#ifdef OUTLINE
    vs_out.out_tex_coord = in_tex_coord;
#endif

    vs_out.window_pos = gl_Position.xyz / gl_Position.w;
}
)"
