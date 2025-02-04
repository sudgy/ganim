#include "scene.hpp"

#include "ganim/gl/gl.hpp"
#include "ganim/util/stb_image_write.h"

using namespace ganim;

Scene::Scene(
    std::string filename,
    int pixel_width,
    int pixel_height,
    double coord_width,
    double coord_height,
    int fps
)
:   SceneBase(pixel_width, pixel_height, coord_width, coord_height, fps),
    M_writer(std::move(filename), pixel_width, pixel_height, fps),
    M_data(std::make_unique<std::uint8_t[]>(pixel_width*pixel_height*3)) {}

void Scene::process_frame()
{
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, pixel_width(), pixel_height(), GL_RGB, GL_UNSIGNED_BYTE,
                 M_data.get());
    auto data = std::span<uint8_t>(
            M_data.get(), pixel_width()*pixel_height()*3);
    M_writer.write_frame(data);
    M_processed = true;
}

void Scene::write_to_image(std::string filename)
{
    if (!M_processed) frame_advance();
    auto real_filename = std::format("{}.png", filename);
    stbi_write_png(
        real_filename.c_str(), pixel_width(), pixel_height(), 3,
        M_data.get(), pixel_width() * 3
    );
}
