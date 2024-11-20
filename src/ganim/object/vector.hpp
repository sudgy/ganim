#ifndef GANIM_OBJECT_VECTOR_HPP
#define GANIM_OBJECT_VECTOR_HPP

#include "bases/single_object.hpp"

#include "ganim/gl/shader.hpp"

namespace ganim {
    struct VectorArgs {
        double max_tip_to_length_ratio = 0.5;
        double tip_size = 0.4;
        double thickness = 0.06;
        bool three_d = false;
    };
    class VectorObject : public SingleObject {
        public:
            explicit VectorObject(pointlike auto p2, VectorArgs args = {})
                : VectorObject(pga3::e123, pointlike_to_pga3(p2), args) {}
            VectorObject(
                    pointlike auto p1, pointlike auto p2, VectorArgs args = {})
                : VectorObject(
                        pointlike_to_pga3(p1), pointlike_to_pga3(p2), args) {}
            VectorObject(
                    pga3::Trivector p1, pga3::Trivector p2, VectorArgs args={});

            VectorObject& set_start(pointlike auto p)
            {
                return set_start(pointlike_to_pga3(p));
            }
            VectorObject& set_start(pga3::Trivector p);
            VectorObject& set_end(pointlike auto p)
            {
                return set_end(pointlike_to_pga3(p));
            }
            VectorObject& set_end(pga3::Trivector p);
            VectorObject& set_start_and_end(pointlike auto p1, pointlike auto p2)
            {
                return set_start_and_end(
                    pointlike_to_pga3(p1),
                    pointlike_to_pga3(p2)
                );
            }
            VectorObject& set_start_and_end(
                    pga3::Trivector p1, pga3::Trivector p2);

            pga2::Bivector get_start_pga2() const;
            pga3::Trivector get_start_pga3() const;
            vga2::Vector get_start_vga2() const;
            vga3::Vector get_start_vga3() const;
            pga2::Bivector get_end_pga2() const;
            pga3::Trivector get_end_pga3() const;
            vga2::Vector get_end_vga2() const;
            vga3::Vector get_end_vga3() const;

            void lock_orientation(bool lock);

            virtual VectorObject& scale(
                const pga3::Trivector& about_point,
                double scale
            ) override;
            using Object::scale;
            virtual VectorObject& apply_rotor(const pga3::Even& rotor) override;
            using Object::apply_rotor;
            virtual Box get_original_true_bounding_box() const override;

            virtual void draw(const Camera& camera) override;

            /** @brief Copy the object for the sake of transformations */
            std::unique_ptr<VectorObject> anim_copy() const;
            /** @brief Interpolate between two Vectors */
            virtual void interpolate(
                const VectorObject& start,
                const VectorObject& end,
                double t
            );

        private:
            gl::Shader* get_shader();

            VectorObject(const VectorObject& other);

            gl::VertexArray M_vertex_array;
            gl::Buffer M_vertex_buffer;
            gl::Buffer M_element_buffer;

            double M_max_tip_to_length_ratio = 0.5;
            double M_tip_size = 0.35;
            bool M_manual_transform = false;
            bool M_animating = false;
            bool M_do_shading = false;
            bool M_lock_orientation = true;
    };
}

#endif
