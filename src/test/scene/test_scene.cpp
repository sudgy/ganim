#include "test_scene.hpp"

#include "ganim/gl/gl.hpp"

TestScene::TestScene(int width, int height)
    : SceneBase(width, height) {}

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
    glReadPixels(0, 0, pixel_width(), pixel_height(), GL_RGB, GL_UNSIGNED_BYTE,
                 new_frame.data());
}
