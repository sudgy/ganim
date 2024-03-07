#include "shaders.hpp"

using namespace ganim;

namespace {
    gl::Shader::Source shape_vertex()
    {
        auto source = gl::Shader::Source();
        source.add_source(
        #include "ganim/shaders/shape_vertex.vert"
        );
        return source;
    }
    gl::Shader::Source shape_fragment()
    {
        auto source = gl::Shader::Source();
        source.add_source(
        #include "ganim/shaders/shape_fragment.frag"
        );
        return source;
    }
    gl::Shader::Source texture_shape_vertex()
    {
        auto source = gl::Shader::Source();
        source.add_source(
        #include "ganim/shaders/texture_shape_vertex.vert"
        );
        return source;
    }
    gl::Shader::Source texture_shape_fragment()
    {
        auto source = gl::Shader::Source();
        source.add_source(
        #include "ganim/shaders/texture_shape_fragment.frag"
        );
        return source;
    }
}

namespace ganim {

gl::Shader& shape_shader()
{
    static auto result = gl::Shader(shape_vertex(), shape_fragment());
    return result;
}

gl::Shader& texture_shape_shader()
{
    static auto result = gl::Shader(
        texture_shape_vertex(),
        texture_shape_fragment()
    );
    return result;
}

}
