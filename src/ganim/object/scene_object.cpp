#include "scene_object.hpp"

#include "ganim/gl/gl.hpp"

using namespace ganim;

using namespace vga2;

SceneObject::SceneObject(
    int pixel_width,
    int pixel_height,
    double coord_width,
    double coord_height,
    int fps
)
:
    TextureShape<PolygonShape>({
        -coord_width / 2 * e1 - coord_height / 2 * e2,
        +coord_width / 2 * e1 - coord_height / 2 * e2,
        +coord_width / 2 * e1 + coord_height / 2 * e2,
        -coord_width / 2 * e1 + coord_height / 2 * e2
    }),
    M_scene(pixel_width, pixel_height, coord_width, coord_height, fps)
{
    set_texture_vertices({{0, 1}, {1, 1}, {1, 0}, {0, 0}});
    set_texture(M_scene.get_framebuffer_texture());
    add_updater([this]{
        M_scene.frame_advance();
    }, true);
}
