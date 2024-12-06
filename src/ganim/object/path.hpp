#ifndef GANIM_OBJECT_PATH_HPP
#define GANIM_OBJECT_PATH_HPP

#include "ganim/object/shape.hpp"

namespace ganim {
    class Path : public Shape {
        public:
            explicit Path(
                const std::vector<pga2::Bivec>& points,
                bool closed = false,
                double thickness = 0.04
            );
            explicit Path(
                const std::vector<vga2::Vec>& points,
                bool closed = false,
                double thickness = 0.04
            );
            void recreate(
                const std::vector<pga2::Bivec>& points,
                bool closed = false,
                double thickness = 0.04
            );
            void recreate(
                const std::vector<vga2::Vec>& points,
                bool closed = false,
                double thickness = 0.04
            );
            void set_dash(double on_time, double off_time);
            virtual ShaderFeature get_shader_flags() override;
            virtual void set_subclass_uniforms(gl::Shader& shader) override;

        private:
            double M_dash_on_time = 0;
            double M_dash_off_time = 0;
    };
}

#endif
