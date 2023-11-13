#ifndef GANIM_TEST_SCENE_H
#define GANIM_TEST_SCENE_H

#include <vector>

#include "ganim/scene/base.hpp"

#include "ganim/color.hpp"

class TestScene : public ganim::SceneBase {
    public:
        TestScene(int width, int height, int fps);
        ganim::Color get_pixel(int t, int x, int y);
        constexpr int time_size() const {return M_data.size();}

    private:
        virtual void process_frame() override;
        std::vector<std::vector<std::uint8_t>> M_data;
};

#endif
