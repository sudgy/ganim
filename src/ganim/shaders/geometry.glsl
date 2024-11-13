R"(
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VertexData {
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
    vec3 true_position;
    float confusing_z;
} gs_in[];

out GeometryData {
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
    float lighting;
    float confusing_z;
} gs_out;

void main() {
    for (int i = 0; i < 3; ++i) {
        gl_Position = gl_in[i].gl_Position;
#ifdef TIME
        gs_out.t = gs_in[i].t;
#endif
#ifdef VERTEX_COLORS
        gs_out.color = gs_in[i].color;
#endif
#ifdef TEXTURE
        gs_out.tex_coord = gs_in[i].tex_coord;
#endif
#ifdef NOISE_CREATE
        gs_out.noise_coord = gs_in[i].noise_coord;
#endif
        gs_out.confusing_z = gs_in[i].confusing_z;
        vec3 a1 = gs_in[1].true_position - gs_in[0].true_position;
        vec3 a2 = gs_in[2].true_position - gs_in[0].true_position;
        vec3 n = normalize(cross(a1, a2));
        gs_out.lighting = max((n.z + 1) / 2, 0.5);
        EmitVertex();
    }
    EndPrimitive();
}
)"
