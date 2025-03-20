#ifndef GANIM_OBJECT_BIVECTOR_HPP
#define GANIM_OBJECT_BIVECTOR_HPP

#include "ganim/object/bases/group.hpp"
#include "ganim/object/polygon_shape.hpp"
#include "ganim/object/arrow_path.hpp"

namespace ganim {
    class Vector;
    /** @brief A struct for the arguments to construct a @ref Bivector. */
    struct BivectorArgs {
        /** @brief The max ratio of the size of the tip to the length of a
         * particular side on the boundary.
         *
         * @see VectorArgs::tip_size
         */
        double max_tip_to_length_ratio = 0.5;
        /** @brief The tip size, that is, the height of the triangle used for
         * the tip on the boundary.
         */
        double tip_size = 0.3;
        /** @brief The thickness of the boundary of the bivector. */
        double thickness = 0.06;
    };
    /** @brief An object representing a VGA bivector.
     *
     * It has tons of different constructors for different ways you might want
     * to make a bivector.  In the end, it is a group composed of two objects:
     * An inside, which is a transparent polygon, and an outside, which is
     * several curves with arrows on the end.
     *
     * @see make_bivector
     */
    class Bivector : public Group {
        public:
            /** @brief Make a bivector from an arbitrary list of points on its
             * boundary using 2D PGA points.
             */
            Bivector(
                const std::vector<pga2::Bivec>& points,
                BivectorArgs args = {}
            );
            /** @brief Make a bivector from an arbitrary list of points on its
             * boundary using 2D VGA points.
             */
            Bivector(
                const std::vector<vga2::Vec>& points,
                BivectorArgs args = {}
            );
            /** @brief Make a bivector from an arbitrary list of points on its
             * boundary using 3D PGA points.
             *
             * All of the vertices must lie roughly in a plane.  If they don't,
             * the behavior is unspecified, and will probably look pretty bad.
             */
            Bivector(
                const std::vector<pga3::Trivec>& points,
                BivectorArgs args = {}
            );
            /** @brief Make a bivector from an arbitrary list of points on its
             * boundary using 3D VGA points.
             *
             * All of the vertices must lie roughly in a plane.  If they don't,
             * the behavior is unspecified, and will probably look pretty bad.
             */
            Bivector(
                const std::vector<vga3::Vec>& points,
                BivectorArgs args = {}
            );
            /** @brief Make a bivector from two vectors, where the resulting
             * bivector is the exterior product of the two input vectors.
             */
            Bivector(
                const Vector& v1,
                const Vector& v2,
                BivectorArgs args = {}
            );
            /** @brief Make a bivector from two vectors, where the resulting
             * bivector is the exterior product of the two input vectors.
             */
            Bivector(
                vga2::Vec v1,
                vga2::Vec v2,
                BivectorArgs args = {}
            );
            /** @brief Make a bivector from two vectors, where the resulting
             * bivector is the exterior product of the two input vectors.
             */
            Bivector(
                vga3::Vec v1,
                vga3::Vec v2,
                BivectorArgs args = {}
            );
            /** @brief Make a bivector object from a 2D VGA bivector.
             *
             * The result will just be a square.
             */
            Bivector(
                vga2::Bivec b,
                BivectorArgs args = {}
            );
            /** @brief Make a bivector object from a 3D VGA bivector.
             *
             * The result will just be a square.
             */
            Bivector(
                vga3::Bivec b,
                BivectorArgs args = {}
            );
            /** @brief Get the objects making up the boundary of the bivector */
            ObjectPtr<Group> get_outside() {return M_outside;}
            /** @brief Get the polygon making up the inside of the bivector */
            ObjectPtr<PolygonShape> get_inside() {return M_inside;}
            virtual Bivector& set_color(Color color) override;

            ObjectPtr<Bivector> polymorphic_copy() const;

        private:
            virtual Bivector* polymorphic_copy_impl() const;

            void common_construct(
                const std::vector<pga2::Bivec>& pga_points,
                const std::vector<vga2::Vec>& vga_points,
                BivectorArgs args
            );
            ObjectPtr<Group> M_outside = nullptr;
            ObjectPtr<PolygonShape> M_inside = nullptr;
            std::vector<ObjectPtr<ArrowPath>> M_outside_paths;
    };

    /** @brief Make a Bivector in an ObjectPtr.
     *
     * @see Bivector::Bivector
     */
    inline ObjectPtr<Bivector> make_bivector(
        const std::vector<pga2::Bivec>& points,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(points, args);
    }
    /** @brief Make a Bivector in an ObjectPtr.
     *
     * @see Bivector::Bivector
     */
    inline ObjectPtr<Bivector> make_bivector(
        const std::vector<vga2::Vec>& points,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(points, args);
    }
    /** @brief Make a Bivector in an ObjectPtr.
     *
     * @see Bivector::Bivector
     */
    inline ObjectPtr<Bivector> make_bivector(
        const std::vector<pga3::Trivec>& points,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(points, args);
    }
    /** @brief Make a Bivector in an ObjectPtr.
     *
     * @see Bivector::Bivector
     */
    inline ObjectPtr<Bivector> make_bivector(
        const std::vector<vga3::Vec>& points,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(points, args);
    }
    /** @brief Make a Bivector in an ObjectPtr.
     *
     * @see Bivector::Bivector
     */
    inline ObjectPtr<Bivector> make_bivector(
        const Vector& v1,
        const Vector& v2,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(v1, v2, args);
    }
    /** @brief Make a Bivector in an ObjectPtr.
     *
     * @see Bivector::Bivector
     */
    inline ObjectPtr<Bivector> make_bivector(
        vga2::Vec v1,
        vga2::Vec v2,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(v1, v2, args);
    }
    /** @brief Make a Bivector in an ObjectPtr.
     *
     * @see Bivector::Bivector
     */
    inline ObjectPtr<Bivector> make_bivector(
        vga3::Vec v1,
        vga3::Vec v2,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(v1, v2, args);
    }
    /** @brief Make a Bivector in an ObjectPtr.
     *
     * @see Bivector::Bivector
     */
    inline ObjectPtr<Bivector> make_bivector(
        vga2::Bivec b,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(b, args);
    }
    /** @brief Make a Bivector in an ObjectPtr.
     *
     * @see Bivector::Bivector
     */
    inline ObjectPtr<Bivector> make_bivector(
        vga3::Bivec b,
        BivectorArgs args = {}
    )
    {
        return ObjectPtr<Bivector>(b, args);
    }
}

#endif
