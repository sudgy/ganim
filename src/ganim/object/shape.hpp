#ifndef GANIM_OBJECT_SHAPE_HPP
#define GANIM_OBJECT_SHAPE_HPP

/** @file
 * @brief Contains the @ref ganim::Shape "Shape" class
 */

#include <vector>

#include "bases/single_object.hpp"

#include "ganim/gl/buffer.hpp"
#include "ganim/gl/vertex_array.hpp"
#include "ganim/gl/shader.hpp"

namespace ganim {
    /** @brief Represents any object that can be thought of as a shape
     *
     * This is a low-level object that can represent practically any shape of
     * any form.  You pass in a ton of vertices representing triangles that make
     * up the shape, and the object represents that shape.  Note that this class
     * can't have any texture.  Use @ref TextureShape for that.
     *
     * Note that while you can directly create instances of this class, you are
     * not intended to do so.  Instead, you should use a subclass either
     * provided by the library or that you created yourself.
     */
    class Shape : public SingleObject {
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
                float t = 0; ///< When to draw this vertex when creating
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
                std::vector<Vertex> vertices,
                std::vector<unsigned> indices
            );
            Shape()=default;
            /** @brief Reset the vertices of this shape.  The parameters are
             * identical to the parameters of the constructor.
             */
            void set_vertices(
                std::vector<Vertex> vertices,
                std::vector<unsigned> indices
            );
            const std::vector<Vertex>& get_vertices() const {return M_vertices;}
            const std::vector<unsigned>& get_indices() const {return M_indices;}
            virtual void draw(const Camera& camera) override;
            void make_invalid() {M_valid = false;}
            virtual gl::Shader* get_shader();
            virtual Box get_original_true_bounding_box() const override;

            GANIM_OBJECT_CHAIN_DECLS(Shape)

        private:
            /** @brief Sends the vertex data to OpenGL.  It's virtual to allow
             * subclasses to change how this happens.  When the function is
             * called, this shape's vertex array will be bound, and this shape's
             * vertex buffer will be bound to `GL_ARRAY_BUFFER`.
             */
            virtual void buffer_vertices();
            /** @brief Sends the index data to OpenGL.  It's virtual to allow
             * subclasses to change how this happens.  When the function is
             * called, this shape's vertex array will be bound, and this shape's
             * element buffer will be bound to `GL_ELEMENT_ARRAY_BUFFER`.
             */
            virtual void buffer_indices();

            std::vector<Vertex> M_vertices;
            std::vector<unsigned> M_indices;
            double M_min_draw_fraction = INFINITY;
            double M_max_draw_fraction = -INFINITY;
            gl::VertexArray M_vertex_array;
            gl::Buffer M_vertex_buffer;
            gl::Buffer M_element_buffer;
            bool M_valid = false;
    };
}

#endif
