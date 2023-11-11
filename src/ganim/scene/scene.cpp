#include "scene.hpp"

#include "ganim/gl/gl.hpp"

using namespace ganim;

Scene::Scene(
    std::string filename,
    int pixel_width,
    int pixel_height,
    int fps
)
:   SceneBase(pixel_width, pixel_height),
    M_writer(std::move(filename), pixel_width, pixel_height, fps),
    M_data(std::make_unique<std::uint8_t[]>(pixel_width*pixel_height*3)) {}

void Scene::process_frame()
{
    glReadPixels(0, 0, pixel_width(), pixel_height(), GL_RGB, GL_UNSIGNED_BYTE,
                 M_data.get());
    auto data = std::span<uint8_t>(
            M_data.get(), pixel_width()*pixel_height()*3);
    M_writer.write_frame(data);
}
