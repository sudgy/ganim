#include "test_scene.hpp"

#include "ganim/gl/gl.hpp"
#include "ganim/util/stb_image_write.h"

TestScene::TestScene(
    int pixel_width, int pixel_height,
    double coord_width, double coord_height,
    int fps
) : SceneBase(pixel_width, pixel_height, coord_width, coord_height, fps) {}

ganim::Color TestScene::get_pixel(int t, int x, int y)
{
    auto frame = M_data.at(t);
    const auto i = (y*pixel_width() + x) * 3;
    return ganim::Color(frame.at(i), frame.at(i+1), frame.at(i+2), 255);
}

void TestScene::process_frame()
{
    auto& new_frame = M_data.emplace_back();
    new_frame.resize(pixel_width() * pixel_height() * 3);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, pixel_width(), pixel_height(), GL_RGB, GL_UNSIGNED_BYTE,
                 new_frame.data());
}

void TestScene::write_frames_to_file(std::string_view filename_base) const
{
    int i = 1;
    for (auto& frame : M_data) {
        auto filename = std::format("{}{}.png", filename_base, i++);
        stbi_write_png(
            filename.c_str(), pixel_width(), pixel_height(), 3,
            frame.data(), pixel_width() * 3
        );
    }
}
