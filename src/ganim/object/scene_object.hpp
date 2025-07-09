#ifndef GANIM_OBJECT_SCENE_OBJECT_HPP
#define GANIM_OBJECT_SCENE_OBJECT_HPP

#include "polygon_shape.hpp"
#include "texture_shape.hpp"
#include "ganim/scene/base.hpp"

namespace ganim {
    class SceneObject : public TextureShape<PolygonShape> {
        public:
            SceneObject(
                int pixel_width,
                int pixel_height,
                double coord_width,
                double coord_height,
                int fps
            );

            constexpr void set_background_color(const Color& color)
                {M_scene.set_background_color(color);}
            void set_transparency_layers(int layers)
                {M_scene.set_transparency_layers(layers);}
            ObjectPtr<Camera> get_camera() {return M_scene.get_camera();}

            template <typename... Ts>
            void add(Ts&&... objects)
            {
                M_scene.add(std::forward<Ts>(objects)...);
            }

            SceneBase& get_scene() {return M_scene;}

        private:
            class SceneObjectScene : public SceneBase {
                public:
                    using SceneBase::SceneBase;

                private:
                    virtual void process_frame() override {}
            };
            SceneObjectScene M_scene;
    };
    inline ObjectPtr<SceneObject> make_scene_object(
        int pixel_width,
        int pixel_height,
        double coord_width,
        double coord_height,
        int fps
    )
    {
        return ObjectPtr<SceneObject>(
            pixel_width,
            pixel_height,
            coord_width,
            coord_height,
            fps
        );
    }
}

#endif
