#include "indicate.hpp"

#include "ganim/object/bases/object.hpp"

void ganim::indicate(
    SceneBase& scene,
    ObjectPtr<Object> object,
    IndicateAnimationArgs args
)
{
    auto new_args = AnimationArgs(
        args.duration,
        [f = std::move(args.rate_function)](double t){
            return rf::there_and_back(t, f);
        }
    );
    auto& indicated = animate(scene, std::move(object), new_args);
    auto center = indicated.get_center<PGA3>();
    indicated.scale(args.scale_factor, center).set_color(args.color);
}
