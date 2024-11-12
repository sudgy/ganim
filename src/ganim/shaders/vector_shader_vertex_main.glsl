R"(
    vec3 in_pos = real_in_pos;
    if (in_pos.x == 0.5) in_pos.x = mid_pos;
    else if (in_pos.x == 1.0) in_pos.x = end_pos;
)"
