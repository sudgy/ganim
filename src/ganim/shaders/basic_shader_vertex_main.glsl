R"(
    vec4[2] r = rotor_mult(model, view);
    vec4 pos = vec4(rotor_trivector_sandwich(r, in_pos*scale), 1.0);
    pos.w = -pos.z;
    pos.x *= camera_scale.x;
    pos.y *= -camera_scale.y; // ffmpeg has the y axis swapped
    pos.z *= pos.z / 4096;
    gl_Position = pos;
    vertex_t = in_t;
    out_color = in_color;
)"
