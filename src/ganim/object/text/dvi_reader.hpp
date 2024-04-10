#ifndef GANIM_OBJECT_DVI_READER_H
#define GANIM_OBJECT_DVI_READER_H

#include <string>
#include <cstdint>
#include <filesystem>

namespace ganim {

struct DVIFont {
    std::string name;
    double mag = 0;
    double size = 0;
    bool system = true;
};

class DVIConsumer {
    public:
        virtual ~DVIConsumer()=default;
        virtual int write_character(
            const DVIFont& font,
            std::uint32_t c,
            int h,
            int v
        )=0;
        virtual void draw_rect(
            int h,
            int v,
            int a,
            int b
        )=0;
        virtual void set_magnification(double mag)=0;
        virtual void process_special(std::string_view special)=0;
};

void read_dvi(std::filesystem::path filename, DVIConsumer& consumer);

}

#endif
