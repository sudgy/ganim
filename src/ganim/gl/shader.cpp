#include "shader.hpp"

#include <array>
#include <iostream>
#include "gl.hpp"

using namespace ganim::gl;

void Shader::Source::add_source(const char* source)
{
    M_source.push_back(source);
}

const std::vector<const char*>& Shader::Source::source() const
{
    return M_source;
}

Shader::Shader(const Source& vertex, const Source& fragment)
{
    constexpr auto C_log_size = std::size_t(512);
    auto info_log = std::array<char, 512>{0};
    int success;
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, vertex.source().size(),
                   vertex.source().data(), nullptr);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, C_log_size, nullptr, info_log.data());
        glDeleteShader(vertex_shader);
        std::cerr << "Couldn't compile vertex shader: "
                  << info_log.data() << "\n";
        return;
    }
    unsigned int fragment_shader
        = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, fragment.source().size(),
                   fragment.source().data(), nullptr);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, C_log_size, nullptr,
                           info_log.data());
        glDeleteShader(fragment_shader);
        std::cerr << "Couldn't compile fragment shader: "
                  << info_log.data() << "\n";
        return;
    }
    M_program_id = glCreateProgram();
    glAttachShader(M_program_id, vertex_shader);
    glAttachShader(M_program_id, fragment_shader);
    glLinkProgram(M_program_id);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glGetProgramiv(M_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(M_program_id, C_log_size, nullptr,
                            info_log.data());
        glDeleteProgram(M_program_id);
        M_program_id = 0;
        std::cerr << "Couldn't link shader: " << info_log.data() << "\n";
    }
}

Shader::~Shader()
{
    if (M_program_id) glDeleteProgram(M_program_id);
}

Shader::Shader(Shader&& other) noexcept : M_program_id(other.M_program_id)
{
    other.M_program_id = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other) {
        M_program_id = other.M_program_id;
        other.M_program_id = 0;
    }
    return *this;
}

int Shader::get_uniform(const char* name) const
{
    auto pos = glGetUniformLocation(M_program_id, name);
    if (pos == -1) {
        std::cerr << "Warning: uniform \"" << name << "\" not found.\n";
    }
    return pos;
}
