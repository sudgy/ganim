#ifndef GANIM_OBJECT_VECTOR_HPP
#define GANIM_OBJECT_VECTOR_HPP

#include "bases/single_object.hpp"

namespace ganim {
    struct VectorArgs {
        double max_tip_to_length_ratio = 0.5;
        double tip_size = 0.35;
    };
    class Vector : public SingleObject {
        public:
            explicit Vector(pointlike auto p2, VectorArgs args = {})
                : Vector(pga3::e123, pointlike_to_pga3(p2), args) {}
            Vector(pointlike auto p1, pointlike auto p2, VectorArgs args = {})
                : Vector(pointlike_to_pga3(p1), pointlike_to_pga3(p2), args) {}
            Vector(pga3::Trivector p1, pga3::Trivector p2, VectorArgs args={});

            Vector& set_start(pointlike auto p)
            {
                return set_start(pointlike_to_pga3(p));
            }
            Vector& set_start(pga3::Trivector p);
            Vector& set_end(pointlike auto p)
            {
                return set_end(pointlike_to_pga3(p));
            }
            Vector& set_end(pga3::Trivector p);
            Vector& set_start_and_end(pointlike auto p1, pointlike auto p2)
            {
                return set_start_and_end(
                    pointlike_to_pga3(p1),
                    pointlike_to_pga3(p2)
                );
            }
            Vector& set_start_and_end(pga3::Trivector p1, pga3::Trivector p2);

            pga2::Bivector get_start_pga2() const;
            pga3::Trivector get_start_pga3() const;
            vga2::Vector get_start_vga2() const;
            vga3::Vector get_start_vga3() const;
            pga2::Bivector get_end_pga2() const;
            pga3::Trivector get_end_pga3() const;
            vga2::Vector get_end_vga2() const;
            vga3::Vector get_end_vga3() const;

            virtual Vector& scale(
                const pga3::Trivector& about_point,
                double scale
            ) override;
            using Object::scale;
            virtual Vector& apply_rotor(const pga3::Even& rotor) override;
            using Object::apply_rotor;
            virtual Box get_original_true_bounding_box() const override;

            virtual void draw(const Camera& camera) override;

        private:
            void reposition();
            void reorient();
            void calculate_size();

            gl::VertexArray M_vertex_array;
            gl::Buffer M_vertex_buffer;
            gl::Buffer M_element_buffer;

            double M_max_tip_to_length_ratio = 0.5;
            double M_tip_size = 0.35;
            double M_tip_pos = 0.0;
            vga3::Vector M_start;
            vga3::Vector M_end;
            bool M_redoing = false;
    };
}

#endif
