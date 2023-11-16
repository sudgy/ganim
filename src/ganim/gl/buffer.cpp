#include "buffer.hpp"

#include "gl.hpp"

using namespace ganim::gl;

Buffer::Buffer()
{
    glGenBuffers(1, &M_id);
}

Buffer::~Buffer()
{
    if (M_id) glDeleteBuffers(1, &M_id);
}

Buffer::Buffer(Buffer&& other) noexcept : M_id(other.M_id)
{
    other.M_id = 0;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept
{
    if (this != &other) {
        M_id = other.M_id;
        other.M_id = 0;
    }
    return *this;
}
