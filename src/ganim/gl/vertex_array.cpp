#include "vertex_array.hpp"

#include "gl.hpp"

using namespace ganim::gl;

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &M_id);
}

VertexArray::~VertexArray()
{
    if (M_id) glDeleteVertexArrays(1, &M_id);
}

VertexArray::VertexArray(VertexArray&& other) noexcept : M_id(other.M_id)
{
    other.M_id = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (this != &other) {
        M_id = other.M_id;
        other.M_id = 0;
    }
    return *this;
}
