#include "test_scene.hpp"

#include <catch2/catch_test_macros.hpp>

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

void TestScene::check_draw_equivalent(
    ganim::ObjectPtr<ganim::Object> o1,
    ganim::ObjectPtr<ganim::Object> o2,
    std::string_view description,
    std::string_view write_to_file_filename
)
{
    if (description != "") {
        INFO(description);
        // This is done with this weird recursion because INFO only works until
        // the end of its scope
        check_draw_equivalent(o1, o2, "", write_to_file_filename);
        return;
    }
    int t = M_data.size();
    add(o1);
    frame_advance();
    remove(*o1);
    add(o2);
    frame_advance();
    remove(*o2);
    if (write_to_file_filename != "") {
        auto object_filename = std::format(
            "{}_object.png",
            write_to_file_filename
        );
        stbi_write_png(
            object_filename.c_str(), pixel_width(), pixel_height(), 3,
            M_data[t].data(), pixel_width() * 3
        );
        auto reference_filename = std::format(
            "{}_reference.png",
            write_to_file_filename
        );
        stbi_write_png(
            reference_filename.c_str(), pixel_width(), pixel_height(), 3,
            M_data[t + 1].data(), pixel_width() * 3
        );
    }
    for (int x = 0; x < pixel_width(); ++x) {
        INFO("x = " << x);
        for (int y = 0; y < pixel_width(); ++y) {
            INFO("y = " << y);
            REQUIRE(get_pixel(t, x, y) == get_pixel(t + 1, x, y));
        }
    }
    M_data.resize(t);
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
