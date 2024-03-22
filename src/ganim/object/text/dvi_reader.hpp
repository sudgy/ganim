#ifndef GANIM_OBJECT_DVI_READER_H
#define GANIM_OBJECT_DVI_READER_H

#include <string>
#include <cstdint>

namespace ganim {

struct DVIFont {
    std::string name;
    double mag = 0;
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
        virtual void set_magnification(double mag)=0;
};

void read_dvi(std::string_view filename, DVIConsumer& consumer);

}

#endif
