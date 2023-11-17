#ifndef GANIM_OBJECT_SHAPE_HPP
#define GANIM_OBJECT_SHAPE_HPP

// This class has several issues that I want to fix so I'll write documentation
// for it later

#include <vector>

#include "object.hpp"

#include "ganim/gl/buffer.hpp"
#include "ganim/gl/vertex_array.hpp"
#include "ganim/gl/shader.hpp"

namespace ganim {
    class Shape : public Object {
        public:
            struct Vertex {
                float x = 0;
                float y = 0;
                float z = 0;
            };
            Shape(
                const std::vector<Vertex>& vertices,
                const std::vector<unsigned> indices
            );
            virtual void draw() override;

        private:
            gl::VertexArray M_vertex_array;
            gl::Buffer M_vertex_buffer;
            gl::Buffer M_element_buffer;
            int M_index_size = -1;
    };
}

#endif
