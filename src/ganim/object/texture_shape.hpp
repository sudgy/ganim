#ifndef GANIM_OBJECT_TEXTURE_SHAPE_HPP
#define GANIM_OBJECT_TEXTURE_SHAPE_HPP

/** @file
 * @brief Contains the @ref ganim::TextureShape class
 */

#include "shape.hpp"

namespace ganim {
    /** @brief Represents a single texture coordinate
     *
     * Note that these are using OpenGL coordinates that are in the
     * range 0-1.
     */
    struct TextureVertex {
        float x = 0; ///< The x coordinate in the texture
        float y = 0; ///< The y coordinate in the texture
    };
    /// @internal
    namespace texture_shape_helper {
        gl::Shader& get_shader();
        void set_texture(unsigned texture);
        void buffer_vertices(
            const Shape& self,
            const std::vector<TextureVertex>& texture_vertices
        );
    }
    /** @brief Represents a shape with a texture.
     *
     * This class is a mixin.  Given a type `T` that derives from @ref Shape,
     * the class `TextureShape<T>` represents that same shape, but with texture
     * capabilities.  Like `Shape`, this is pretty low-level and requires
     * knowledge of the exact vertex layout of the shape.
     *
     * Note that this class doesn't actually own the texture.  If you want to
     * own the texture, make your own class.
     *
     * @tparam T The shape type to add texture capabilities to.  It must be
     * derived from @ref Shape, or be `Shape` itself.
     */
    template <std::derived_from<Shape> T>
    class TextureShape : public T {
        public:
            using T::T;
            /** @brief Set the texture coordinates for all the vertices in the
             * shape.
             *
             * Note that @ref TextureVertex uses OpenGL coordinates that go zero
             * to one, not image coordinates.
             *
             * @param texture_vertices A vector where each elements represents
             * the texture coordinates of a particular vertex.  The length of
             * this vector must be the same as the length of the vector of
             * vertices passed to the Shape constructor or to @ref
             * Shape::set_vertices.  If there is a mismatch between the two at
             * the time the shape is drawn, the behavior is undefined.
             */
            void set_texture_vertices(
                std::vector<TextureVertex> texture_vertices
            )
            {
                M_texture_vertices = std::move(texture_vertices);
                this->make_invalid();
            }
            /** @brief Set the texture used by this shape.
             *
             * This will not own the texture.
             *
             * @param texture The OpenGL texture id of the texture.
             */
            void set_texture(unsigned texture)
            {
                M_texture = texture;
            }
            virtual void draw() override
            {
                texture_shape_helper::set_texture(M_texture);
                T::draw();
            }

            GANIM_OBJECT_CHAIN_DECLS(TextureShape);

        private:
            virtual void buffer_vertices() override
            {
                texture_shape_helper::buffer_vertices(
                    *this,
                    M_texture_vertices
                );
            }
            virtual gl::Shader& get_shader() override
            {
                return texture_shape_helper::get_shader();
            }

            std::vector<TextureVertex> M_texture_vertices;
            unsigned M_texture;
    };
}

#endif
