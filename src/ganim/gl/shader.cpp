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
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    compile_part(vertex_shader, vertex, "vertex");
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    compile_part(fragment_shader, fragment, "fragment");
    M_program_id = glCreateProgram();
    glAttachShader(M_program_id, vertex_shader);
    glAttachShader(M_program_id, fragment_shader);
    glLinkProgram(M_program_id);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    int success = 0;
    glGetProgramiv(M_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        constexpr auto C_log_size = std::size_t(512);
        auto info_log = std::array<char, 512>{0};
        glGetProgramInfoLog(M_program_id, C_log_size, nullptr,
                            info_log.data());
        glDeleteProgram(M_program_id);
        M_program_id = 0;
        std::cerr << "Couldn't link shader: " << info_log.data() << "\n";
    }
}

Shader::Shader(const Source& compute)
{
    constexpr auto C_log_size = std::size_t(512);
    auto info_log = std::array<char, 512>{0};
    unsigned int compute_shader = glCreateShader(GL_COMPUTE_SHADER);
    if (!compile_part(compute_shader, compute, "compute")) return;
    M_program_id = glCreateProgram();
    glAttachShader(M_program_id, compute_shader);
    glLinkProgram(M_program_id);
    glDeleteShader(compute_shader);
    int success = 0;
    glGetProgramiv(M_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(M_program_id, C_log_size, nullptr,
                            info_log.data());
        glDeleteProgram(M_program_id);
        M_program_id = 0;
        std::cerr << "Couldn't link shader: " << info_log.data() << "\n";
    }
}

bool Shader::compile_part(
    unsigned int shader,
    const Source& source,
    std::string_view type
)
{
    glShaderSource(
        shader, source.source().size(), source.source().data(), nullptr
    );
    glCompileShader(shader);
    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        constexpr auto C_log_size = std::size_t(512);
        auto info_log = std::array<char, C_log_size>{0};
        glGetShaderInfoLog(shader, C_log_size, nullptr, info_log.data());
        glDeleteShader(shader);
        std::cerr << "Couldn't compile " << type << " shader: "
                  << info_log.data() << "\n";
        return false;
    }
    return true;
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

void Shader::set_rotor_uniform(const char* name, const pga3::Even& rotor)
{
    using namespace pga3;
    auto pos = get_uniform(name);
    auto vals = std::array<float, 8>{
        static_cast<float>(rotor.blade_project<e>()),
        static_cast<float>(rotor.blade_project<e23>()),
        static_cast<float>(rotor.blade_project<e31>()),
        static_cast<float>(rotor.blade_project<e12>()),
        static_cast<float>(rotor.blade_project<e01>()),
        static_cast<float>(rotor.blade_project<e02>()),
        static_cast<float>(rotor.blade_project<e03>()),
        static_cast<float>(rotor.blade_project<e0123>())
    };
    glUniform4fv(pos, 2, vals.data());
}
