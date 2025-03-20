#ifndef GANIM_OBJECT_POLYGON_SHAPE_HPP
#define GANIM_OBJECT_POLYGON_SHAPE_HPP

#include "ganim/object/shape.hpp"

namespace ganim {
    /** @brief An object representing a single polygon.
     */
    class PolygonShape : public Shape {
        public:
            /** @brief Create a polygon with the given vertices. */
            explicit PolygonShape(const std::vector<vga2::Vec>& vertices);
            /** @brief Create a polygon with the given vertices. */
            explicit PolygonShape(const std::vector<pga2::Bivec>& vertices);
            /** @brief Create a polygon with the given 3D vertices.
             *
             * All of the vertices must lie roughly in a plane.  If they don't,
             * the behavior is unspecified, and will probably look pretty bad.
             */
            explicit PolygonShape(const std::vector<vga3::Vec>& vertices);
            /** @brief Create a polygon with the given 3D vertices.
             *
             * All of the vertices must lie roughly in a plane.  If they don't,
             * the behavior is unspecified, and will probably look pretty bad.
             */
            explicit PolygonShape(const std::vector<pga3::Trivec>& vertices);

            ObjectPtr<PolygonShape> polymorphic_copy() const;

        private:
            virtual PolygonShape* polymorphic_copy_impl() const;
    };
    /** @brief Make a PolygonShape in an ObjectPtr.
     *
     * @see PolygonShape::PolygonShape
     */
    inline ObjectPtr<PolygonShape> make_polygon_shape(
        const std::vector<vga2::Vec>& vertices
    )
    {
        return ObjectPtr<PolygonShape>(vertices);
    }
    /** @brief Make a PolygonShape in an ObjectPtr.
     *
     * @see PolygonShape::PolygonShape
     */
    inline ObjectPtr<PolygonShape> make_polygon_shape(
        const std::vector<pga2::Bivec>& vertices
    )
    {
        return ObjectPtr<PolygonShape>(vertices);
    }
    /** @brief Make a PolygonShape in an ObjectPtr.
     *
     * @see PolygonShape::PolygonShape
     */
    inline ObjectPtr<PolygonShape> make_polygon_shape(
        const std::vector<vga3::Vec>& vertices
    )
    {
        return ObjectPtr<PolygonShape>(vertices);
    }
    /** @brief Make a PolygonShape in an ObjectPtr.
     *
     * @see PolygonShape::PolygonShape
     */
    inline ObjectPtr<PolygonShape> make_polygon_shape(
        const std::vector<pga3::Trivec>& vertices
    )
    {
        return ObjectPtr<PolygonShape>(vertices);
    }
}

#endif
