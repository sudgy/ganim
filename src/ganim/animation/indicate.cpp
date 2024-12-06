#include "indicate.hpp"

#include "ganim/object/bases/object.hpp"

void ganim::indicate(
    SceneBase& scene,
    MaybeOwningRef<Object> object,
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
    auto center = indicated.get_logical_bounding_box().get_center();
    indicated.scale(args.scale_factor, center).set_color(Color("FFFF00"));
}
