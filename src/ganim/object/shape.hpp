#ifndef GANIM_OBJECT_SHAPE_HPP
#define GANIM_OBJECT_SHAPE_HPP

/** @file
 * @brief Contains the @ref ganim::Shape "Shape" class
 */

#include <vector>

#include "drawable.hpp"

#include "ganim/gl/buffer.hpp"
#include "ganim/gl/vertex_array.hpp"
#include "ganim/gl/shader.hpp"

namespace ganim {
    /** @brief Represents any object that can be thought of as a shape
     *
     * This is a low-level object that can represent practically any shape of
     * any form.  You pass in a ton of vertices representing triangles that make
     * up the shape, and the object represents that shape.  Note that this class
     * can't have any texture.  I'll make a separate class for that later.
     *
     * Note that while you can directly create instances of this class, you are
     * not intended to do so.  Instead, you should use a subclass either
     * provided by the library or that you created yourself.
     */
    class Shape : public Drawable {
        public:
            /** @brief Represents a single vertex in this shape
             *
             * As floats, these colors are in the range 0-1, not 0-255.  The
             * colors will get interpolated by OpenGL.
             */
            struct Vertex {
                float x = 0; ///< The x coordinate of the vertex
                float y = 0; ///< The y coordinate of the vertex
                float z = 0; ///< The z coordinate of the vertex
                float r = 1; ///< The red component of the vertex
                float g = 1; ///< The green component of the vertex
                float b = 1; ///< The blue component of the vertex
                float a = 1; ///< The alpha component of the vertex
            };
            /** @brief Constructor.
             *
             * @param vertices The vertices that are present in this shape.
             * @param indices The indices into the `vertices` parameter of the
             * triangles in this shape.  Every element of this vector must be a
             * valid index into `vertices`.  If not, the behavior is undefined.
             */
            Shape(
                const std::vector<Vertex>& vertices,
                const std::vector<unsigned> indices
            );
            virtual void draw() override;

            GANIM_TRANSFORMABLE_CHAIN_DECLS(Shape)

        private:
            gl::VertexArray M_vertex_array;
            gl::Buffer M_vertex_buffer;
            gl::Buffer M_element_buffer;
            int M_index_size = -1;
    };
}

#endif
