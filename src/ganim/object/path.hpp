#ifndef GANIM_OBJECT_PATH_HPP
#define GANIM_OBJECT_PATH_HPP

#include "ganim/object/shape.hpp"

namespace ganim {
    /** @brief An object representing a 2D curve.
     *
     * Curves are technically represented as a sequence of thin trapezoids.
     */
    class Path : public Shape {
        public:
            /** @brief Construct a path given a list of points using 2D PGA.
             *
             * @param points The points that this path will follow
             * @param closed Whether these points represent a closed curve or
             * not.  If this parameter is true, there will be one extra line
             * joining the first and last points of the path, making it a closed
             * curve.
             * @param thickness The width of the trapezoids making up the path.
             */
            explicit Path(
                const std::vector<pga2::Bivec>& points,
                bool closed = false,
                double thickness = 0.04
            );
            /** @brief Construct a path given a list of points using 2D PGA.
             *
             * @param points The points that this path will follow
             * @param closed Whether these points represent a closed curve or
             * not.  If this parameter is true, there will be one extra line
             * joining the first and last points of the path, making it a closed
             * curve.
             * @param thickness The width of the trapezoids making up the path.
             */
            explicit Path(
                const std::vector<vga2::Vec>& points,
                bool closed = false,
                double thickness = 0.04
            );
            /** @brief Construct a 3D path given a list of points using 3D PGA.
             *
             * @param points The points that this path will follow
             * @param circle_precision The number of sides on the polygons used
             * to represent the cylindrical shape of the path.
             * @param closed Whether these points represent a closed curve or
             * not.  If this parameter is true, there will be one extra line
             * joining the first and last points of the path, making it a closed
             * curve.
             * @param thickness The width of the path.
             */
            explicit Path(
                const std::vector<pga3::Trivec>& points,
                int circle_precision,
                bool closed = false,
                double thickness = 0.04
            );
            /** @brief Construct a 3D path given a list of points using 3D VGA.
             *
             * @param points The points that this path will follow
             * @param circle_precision The number of sides on the polygons used
             * to represent the cylindrical shape of the path.
             * @param closed Whether these points represent a closed curve or
             * not.  If this parameter is true, there will be one extra line
             * joining the first and last points of the path, making it a closed
             * curve.
             * @param thickness The width of the path.
             */
            explicit Path(
                const std::vector<vga3::Vec>& points,
                int circle_precision,
                bool closed = false,
                double thickness = 0.04
            );
            /** @brief Basically the same as "recalling" the constructor.
             *
             * @see Path(const std::vector<pga2::Bivec>&, bool, double)
             */
            void recreate(
                const std::vector<pga2::Bivec>& points,
                bool closed = false,
                double thickness = 0.04
            );
            /** @brief Basically the same as "recalling" the constructor.
             *
             * @see Path(const std::vector<vga2::Vec>&, bool, double)
             */
            void recreate(
                const std::vector<vga2::Vec>& points,
                bool closed = false,
                double thickness = 0.04
            );
            /** @brief Basically the same as "recalling" the constructor.
             *
             * @see Path(const std::vector<pga2::Trivec>&, int, bool, double)
             */
            void recreate(
                const std::vector<pga3::Trivec>& points,
                int circle_precision,
                bool closed = false,
                double thickness = 0.04
            );
            /** @brief Basically the same as "recalling" the constructor.
             *
             * @see Path(const std::vector<vga2::Vec>&, int, bool, double)
             */
            void recreate(
                const std::vector<vga3::Vec>& points,
                int circle_precision,
                bool closed = false,
                double thickness = 0.04
            );
            /** @brief Set if this path should be dashed, and how it should be
             * dashed.
             *
             * Each point passed to the cosntructor is given successive t
             * values, which will then get interpolated across each part of the
             * path.  Then, after calling this function, the pixels between `0`
             * and `on_time`, `on_time*2` and `on_time*3`, and so on will be
             * drawn, and the pixels between `on_time` and `on_time*2`,
             * `on_time*3` and `on_time*4`, and so on will not be drawn.  Pass
             * in zero to disable drawing with dashes.
             */
            void set_dash(double on_time, double off_time);
            virtual ShaderFeature get_shader_flags() override;
            virtual void set_subclass_uniforms(gl::Shader& shader) override;

            ObjectPtr<Path> polymorphic_copy() const;

        private:
            virtual Path* polymorphic_copy_impl() const;

            double M_dash_on_time = 0;
            double M_dash_off_time = 0;
    };
    /** @brief Make a Path in an ObjectPtr.
     *
     * @see Path::Path
     */
    inline ObjectPtr<Path> make_path(
        const std::vector<pga2::Bivec>& points,
        bool closed = false,
        double thickness = 0.04
    )
    {
        return ObjectPtr<Path>(points, closed, thickness);
    }
    /** @brief Make a Path in an ObjectPtr.
     *
     * @see Path::Path
     */
    inline ObjectPtr<Path> make_path(
        const std::vector<vga2::Vec>& points,
        bool closed = false,
        double thickness = 0.04
    )
    {
        return ObjectPtr<Path>(points, closed, thickness);
    }
    /** @brief Make a Path in an ObjectPtr.
     *
     * @see Path::Path
     */
    inline ObjectPtr<Path> make_path(
        const std::vector<pga3::Trivec>& points,
        int circle_precision,
        bool closed = false,
        double thickness = 0.04
    )
    {
        return ObjectPtr<Path>(points, circle_precision, closed, thickness);
    }
    /** @brief Make a Path in an ObjectPtr.
     *
     * @see Path::Path
     */
    inline ObjectPtr<Path> make_path(
        const std::vector<vga3::Vec>& points,
        int circle_precision,
        bool closed = false,
        double thickness = 0.04
    )
    {
        return ObjectPtr<Path>(points, circle_precision, closed, thickness);
    }
}

#endif
