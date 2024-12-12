#ifndef GANIM_SINGLE_OBJECT_HPP
#define GANIM_SINGLE_OBJECT_HPP

#include "object.hpp"

#include "ganim/gl/texture.hpp"
#include "ganim/gl/vertex_array.hpp"
#include "ganim/gl/buffer.hpp"

namespace ganim {

class SingleObject : public Object {
    public:
        SingleObject()=default;
        SingleObject(const SingleObject&);
        SingleObject(SingleObject&&) noexcept=default;
        SingleObject& operator=(const SingleObject&);
        SingleObject& operator=(SingleObject&&) noexcept=default;
        virtual bool is_visible() const override
        {
            return Object::is_visible();
        }
        virtual void draw_outline(const Camera& camera) override;
        virtual void set_outline(const Color& color, double thickness) override;
        virtual void invalidate_outline() override;
        virtual Color get_outline_color() const override;
        virtual double get_outline_thickness() const override;
        virtual void set_draw_fraction(double value) override;

    private:
        void create_outline(const Camera& camera);

        Color M_outline_color;
        double M_outline_thickness = 0;
        // TODO: Share this with other objects that look the same
        gl::Texture M_outline_texture = 0;
        gl::VertexArray M_outline_vertex_array = 0;
        gl::Buffer M_outline_vertex_buffer = 0;
        gl::Buffer M_outline_element_buffer = 0;
        bool M_always_invalidate_outline = 0;
};

}

#endif
