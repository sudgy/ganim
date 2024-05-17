R"(
    vec2 unused_noise_gradient;
    float final_t = this_t + noise_scale * psrdnoise(
        noise_coord,
        vec2(0.0, 0.0),
        0.0,
        unused_noise_gradient
    );
    if (final_t < vertex_t) discard;
)"
