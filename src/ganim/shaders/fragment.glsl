R"(
#ifdef FACE_SHADING
in GeometryData {
#else
in VertexData {
#endif
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
    float lighting;
#endif
    vec3 window_pos;
} fs_in;

#ifdef TEXTURE
uniform sampler2D in_texture;
#endif
#ifdef CREATE
uniform float this_t;
#endif
#ifdef NOISE_CREATE
uniform float this_t;
uniform float noise_scale;
#endif
#ifdef DEPTH_PEELING
uniform sampler2DMS layer_depth_buffer;
#endif

out vec4 color;

uniform vec4 object_color;

// Various helper functions

// psrdnoise (c) Stefan Gustavson and Ian McEwan,
// ver. 2021-12-02, published under the MIT license:
// https://github.com/stegu/psrdnoise/
float psrdnoise(vec2 x, vec2 period, float alpha, out vec2 gradient)
{
  vec2 uv = vec2(x.x+x.y*0.5, x.y);
  vec2 i0 = floor(uv), f0 = fract(uv);
  float cmp = step(f0.y, f0.x);
  vec2 o1 = vec2(cmp, 1.0-cmp);
  vec2 i1 = i0 + o1, i2 = i0 + 1.0;
  vec2 v0 = vec2(i0.x - i0.y*0.5, i0.y);
  vec2 v1 = vec2(v0.x + o1.x - o1.y*0.5, v0.y + o1.y);
  vec2 v2 = vec2(v0.x + 0.5, v0.y + 1.0);
  vec2 x0 = x - v0, x1 = x - v1, x2 = x - v2;
  vec3 iu, iv, xw, yw;
  if(any(greaterThan(period, vec2(0.0)))) {
    xw = vec3(v0.x, v1.x, v2.x);
    yw = vec3(v0.y, v1.y, v2.y);
    if(period.x > 0.0)
    xw = mod(vec3(v0.x, v1.x, v2.x), period.x);
    if(period.y > 0.0)
      yw = mod(vec3(v0.y, v1.y, v2.y), period.y);
    iu = floor(xw + 0.5*yw + 0.5); iv = floor(yw + 0.5);
  } else {
    iu = vec3(i0.x, i1.x, i2.x); iv = vec3(i0.y, i1.y, i2.y);
  }
  vec3 hash = mod(iu, 289.0);
  hash = mod((hash*51.0 + 2.0)*hash + iv, 289.0);
  hash = mod((hash*34.0 + 10.0)*hash, 289.0);
  vec3 psi = hash*0.07482 + alpha;
  vec3 gx = cos(psi); vec3 gy = sin(psi);
  vec2 g0 = vec2(gx.x, gy.x);
  vec2 g1 = vec2(gx.y, gy.y);
  vec2 g2 = vec2(gx.z, gy.z);
  vec3 w = 0.8 - vec3(dot(x0, x0), dot(x1, x1), dot(x2, x2));
  w = max(w, 0.0); vec3 w2 = w*w; vec3 w4 = w2*w2;
  vec3 gdotx = vec3(dot(g0, x0), dot(g1, x1), dot(g2, x2));
  float n = dot(w4, gdotx);
  vec3 w3 = w2*w; vec3 dw = -8.0*w3*gdotx;
  vec2 dn0 = w4.x*g0 + dw.x*x0;
  vec2 dn1 = w4.y*g1 + dw.y*x1;
  vec2 dn2 = w4.z*g2 + dw.z*x2;
  gradient = 10.9*(dn0 + dn1 + dn2);
  return 10.9*n;
}

void main()
{
#ifdef CREATE
    if (this_t < fs_in.t) discard;
#endif
#ifdef NOISE_CREATE
    vec2 unused_noise_gradient;
    float final_t = this_t + noise_scale * psrdnoise(
        fs_in.noise_coord,
        vec2(0.0, 0.0),
        0.0,
        unused_noise_gradient
    );
    if (final_t < fs_in.t) discard;
#endif
#ifdef DEPTH_PEELING
    ivec2 depth_pos = ivec2(
        round(gl_FragCoord.x - 0.5),
        round(gl_FragCoord.y - 0.5)
    );
    float depth = 0;
    for (int i = 0; i < 4; ++i) {
        depth += texelFetch(layer_depth_buffer, depth_pos, i).r;
    }
    depth /= 4;
    if (depth >= fs_in.window_pos.z) discard;
#endif
    color = object_color;
#ifdef VERTEX_COLORS
    color *= fs_in.color;
#endif
#ifdef TEXTURE
    color *= texture(in_texture, fs_in.tex_coord);
#endif
#ifdef FACE_SHADING
    color.xyz *= fs_in.lighting;
#endif
    gl_FragDepth = fs_in.window_pos.z;
}
)"
