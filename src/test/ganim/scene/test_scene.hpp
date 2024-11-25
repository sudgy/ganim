#ifndef GANIM_TEST_SCENE_HPP
#define GANIM_TEST_SCENE_HPP

#include <vector>

#include "ganim/scene/base.hpp"

#include "ganim/color.hpp"

class TestScene : public ganim::SceneBase {
    public:
        TestScene(
            int pixel_width, int pixel_height,
            double coord_width, double coord_height,
            int fps
        );
        ganim::Color get_pixel(int t, int x, int y);
        void check_draw_equivalent(
            ganim::Object& o1,
            ganim::Object& o2,
            std::string_view description,
            std::string_view write_to_file_filename = ""
        );
        constexpr int time_size() const {return M_data.size();}
        void write_frames_to_file(std::string_view filename_base) const;

    private:
        virtual void process_frame() override;
        std::vector<std::vector<std::uint8_t>> M_data;
};

#endif
