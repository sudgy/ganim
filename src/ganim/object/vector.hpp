#ifndef GANIM_OBJECT_VECTOR_HPP
#define GANIM_OBJECT_VECTOR_HPP

#include "bases/single_object.hpp"

#include "ganim/gl/shader.hpp"

namespace ganim {
    /** @brief A struct for the arguments to construct a @ref Vector. */
    struct VectorArgs {
        /** @brief The maximum size of the tip of the arrow relative to its
         * total length
         *
         * If the vector gets smaller than this, the tip will get smaller as
         * well.
         */
        double max_tip_to_length_ratio = 0.5;
        /** @brief The default size of the tip of the arrow
         *
         * This size is measured as the height of the triangle making up the
         * tip.
         */
        double tip_size = 0.4;
        /** @brief The thickness of the long part of the arrow
         */
        double thickness = 0.06;
        /** @brief Whether this vector should be drawn three-dimensionally.
         *
         * 3D vectors have a bit of added thickness to make them look better in
         * 3D.
         */
        bool three_d = false;
    };
    class Vector : public SingleObject {
        public:
            /** @brief Create a vector starting at the origin pointing to a
             * particular point.
             */
            explicit Vector(pointlike auto p2, VectorArgs args = {})
                : Vector(pga3::e123, pointlike_to_pga3(p2), args) {}
            /** @brief Create a vector going between two points. */
            Vector(pointlike auto p1, pointlike auto p2, VectorArgs args = {})
                : Vector(pointlike_to_pga3(p1), pointlike_to_pga3(p2), args) {}
            /** @brief Create a vector going between two points. */
            Vector(pga3::Trivec p1, pga3::Trivec p2, VectorArgs args={});

            /** @brief Get the starting point of the vector. */
            Vector& set_start(pointlike auto p)
            {
                return set_start(pointlike_to_pga3(p));
            }
            /** @brief Set the starting point of the vector. */
            Vector& set_start(pga3::Trivec p);
            /** @brief Get the ending point of the vector. */
            Vector& set_end(pointlike auto p)
            {
                return set_end(pointlike_to_pga3(p));
            }
            /** @brief Set the ending point of the vector. */
            Vector& set_end(pga3::Trivec p);
            /** @brief Set both the starting and ending points of the vector. */
            Vector& set_start_and_end(pointlike auto p1, pointlike auto p2)
            {
                return set_start_and_end(
                    pointlike_to_pga3(p1),
                    pointlike_to_pga3(p2)
                );
            }
            /** @brief Set both the starting and ending points of the vector. */
            Vector& set_start_and_end(
                    pga3::Trivec p1, pga3::Trivec p2);

            pga2::Bivec get_start_pga2() const;
            pga3::Trivec get_start_pga3() const;
            vga2::Vec get_start_vga2() const;
            vga3::Vec get_start_vga3() const;
            pga2::Bivec get_end_pga2() const;
            pga3::Trivec get_end_pga3() const;
            vga2::Vec get_end_vga2() const;
            vga3::Vec get_end_vga3() const;

            /** @brief Set whether to try to keep the vector facing the xy plane
             *
             * When moving a vector around in 3D, there are two main strategies
             * that can be used for orientation: Let it do whatever it wants, or
             * try to keep it facing a certain direction, like the xy plane.
             * The default here is to face the xy plane, but if you don't want
             * this to happen, pass false to this function.
             */
            void lock_orientation(bool lock);

            virtual Vector& scale(
                double scale,
                const pga3::Trivec& about_point
            ) override;
            using Object::scale;
            virtual Vector& apply_rotor(const pga3::Even& rotor) override;
            using Object::apply_rotor;
            virtual Box get_original_true_bounding_box() const override;

            virtual void draw(const Camera& camera) override;

            /** @brief Interpolate between two Vectors */
            virtual void interpolate(
                const Animatable& start,
                const Animatable& end,
                double t
            ) override;
            ObjectPtr<Vector> polymorphic_copy() const;

        private:
            virtual Vector* polymorphic_copy_impl() const override;
            gl::Shader* get_shader();

            Vector(const Vector& other);

            gl::VertexArray M_vertex_array;
            gl::Buffer M_vertex_buffer;
            gl::Buffer M_element_buffer;

            double M_max_tip_to_length_ratio = 0.5;
            double M_tip_size = 0.35;
            bool M_manual_transform = false;
            bool M_animating = false;
            bool M_do_shading = false;
            bool M_lock_orientation = true;
            bool M_3d = false;
    };

    /** @brief Make a Vector in an ObjectPtr.
     *
     * @see Vector::Vector
     */
    inline ObjectPtr<Vector> make_vector(pointlike auto p2, VectorArgs args = {})
    {
        return ObjectPtr<Vector>(p2, args);
    }
    /** @brief Make a Vector in an ObjectPtr.
     *
     * @see Vector::Vector
     */
    inline ObjectPtr<Vector> make_vector(
            pointlike auto p1, pointlike auto p2, VectorArgs args = {})
    {
        return ObjectPtr<Vector>(p1, p2, args);
    }
    /** @brief Make a Vector in an ObjectPtr.
     *
     * @see Vector::Vector
     */
    inline ObjectPtr<Vector> make_vector(
            pga3::Trivec p1, pga3::Trivec p2, VectorArgs args = {})
    {
        return ObjectPtr<Vector>(p1, p2, args);
    }
}

#endif
