#include "alpha_threshold.hpp"

#include <format>

#include "ganim/gl/shader.hpp"
#include "ganim/gl/gl.hpp"

using namespace ganim;

namespace {
    const char* shader_source =
R"(
#version 430 core

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

uniform layout(rgba32f, binding = 0) readonly image2D input_img;
uniform layout(r8ui, binding = 1) writeonly uimage2D output_img;
uniform layout(location = 2) float threshold;

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    float alpha = imageLoad(input_img, pos).a;
    float red = alpha >= threshold ? 255 : 0;
    imageStore(output_img, pos, uvec4(red, 0, 0, 0));
}
)";
    gl::Shader make_shader()
    {
        auto source = gl::Shader::Source();
        source.add_source(shader_source);
        return gl::Shader(source);
    }
    gl::Shader& shader()
    {
        static auto result = make_shader();
        return result;
    }
}

gl::Texture ganim::alpha_threshold(
    const gl::Texture& tex,
    float threshold,
    int width,
    int height
)
{
    if (width <= 0) throw std::invalid_argument(std::format(
        "The texture width passed to alpha_threshold is not positive: {}",
        width));
    if (height <= 0) throw std::invalid_argument(std::format(
        "The texture height passed to alpha_threshold is not positive: {}",
        height));
    if (width % 8 != 0) throw std::invalid_argument(std::format(
        "The texture width passed to alpha_threshold is not a multiple of 8: "
        "{}", width));
    if (height % 8 != 0) throw std::invalid_argument(std::format(
        "The texture height passed to alpha_threshold is not a multiple of 8: "
        "{}", height));

    auto& shader = ::shader();
    glUseProgram(shader);

    glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

    auto result = gl::Texture();
    glBindTexture(GL_TEXTURE_2D, result);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8UI, width, height);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindImageTexture(1, result, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R8UI);

    glUniform1f(2, threshold);

    glDispatchCompute(width / 8, height / 8, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    return result;
}
