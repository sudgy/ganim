#include "distance_transform.hpp"

#include <format>
#include <bit>

#include "ganim/gl/shader.hpp"
#include "ganim/gl/gl.hpp"

using namespace ganim;

namespace {
    const char* start_source =
R"(
#version 430 core

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

uniform layout(r8ui, binding = 0) readonly uimage2D input_img;
uniform layout(rgba32f, binding = 1) writeonly image2D output_img;

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    if (imageLoad(input_img, pos).r == 255) {
        imageStore(output_img, pos, vec4(pos.xy, 0.0, 0.0));
    }
    else {
        imageStore(output_img, pos, vec4(0.0, 0.0, 0.0, 1.0 / 0.0));
    }
}
)";
    const char* main_source =
R"(
#version 430 core

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

uniform layout(rgba32f, binding = 0) readonly image2D input_img;
uniform layout(rgba32f, binding = 1) writeonly image2D output_img;
uniform layout(location = 2) int n;
uniform layout(location = 3) int size;

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    vec4 p = imageLoad(input_img, pos);
    if (p.a != 0.0) {
        for (int x = -n; x <= n; x += n) {
            if (pos.x + x < 0 || pos.x + x >= size) continue;
            for (int y = -n; y <= n; y += n) {
                if (pos.y + y < 0 || pos.y + y >= size) continue;
                if (x == 0 && y == 0) continue;
                vec4 q = imageLoad(input_img, pos + ivec2(x, y));
                if (isinf(q.a)) continue;
                float q_distance = distance(pos, q.xy);
                if (q_distance < p.a) {
                    p.xy = q.xy;
                    p.a = q_distance;
                }
            }
        }
    }
    imageStore(output_img, pos, p);
}
)";
    const char* end_source =
R"(
#version 430 core

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

uniform layout(rgba32f, binding = 0) readonly image2D input_img;
uniform layout(r32f, binding = 1) writeonly image2D output_img;
uniform float scale;

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    imageStore(
        output_img,
        pos,
        vec4(imageLoad(input_img, pos).a * scale, 0, 0, 0)
    );
}
)";
    gl::Shader make_start()
    {
        auto source = gl::Shader::Source();
        source.add_source(start_source);
        return gl::Shader(source);
    }
    gl::Shader make_main()
    {
        auto source = gl::Shader::Source();
        source.add_source(main_source);
        return gl::Shader(source);
    }
    gl::Shader make_end()
    {
        auto source = gl::Shader::Source();
        source.add_source(end_source);
        return gl::Shader(source);
    }
    gl::Shader& start_shader()
    {
        static auto result = make_start();
        return result;
    }
    gl::Shader& main_shader()
    {
        static auto result = make_main();
        return result;
    }
    gl::Shader& end_shader()
    {
        static auto result = make_end();
        return result;
    }
}

gl::Texture ganim::distance_transform(
    const gl::Texture& input,
    int size,
    double scale
)
{
    if (size <= 0) throw std::invalid_argument(std::format(
        "The texture size passed to distance_transform is not positive: {}",
        size));
    if (!std::has_single_bit(static_cast<unsigned>(size)))
        throw std::invalid_argument(std::format(
            "The texture size passed to alpha_threshold is not a multiple of 8:"
            "{}", size));

    auto& start = start_shader();
    auto& main = main_shader();
    auto& end = end_shader();

    auto tex1 = gl::Texture();
    auto tex2 = gl::Texture();
    glBindTexture(GL_TEXTURE_2D, tex1);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, size, size);
    glBindTexture(GL_TEXTURE_2D, tex2);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, size, size);
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(start);
    glBindImageTexture(0, input, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R8UI);
    glBindImageTexture(1, tex1, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(size / 8, size / 8, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glUseProgram(main);
    glUniform1i(3, size);
    for (int n = size / 2; n > 0; n /= 2) {
        glBindImageTexture(0, tex1, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindImageTexture(1, tex2, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glUniform1i(2, n);
        glDispatchCompute(size / 8, size / 8, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        std::swap(tex1, tex2);
    }

    glUseProgram(end);
    auto result = gl::Texture();
    glBindTexture(GL_TEXTURE_2D, result);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, size, size);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindImageTexture(0, tex1, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(1, result, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
    glUniform1f(end.get_uniform("scale"), scale);
    glDispatchCompute(size / 8, size / 8, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glBindTexture(GL_TEXTURE_2D, result);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR
    );
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    return result;
}
