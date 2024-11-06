#include "ganim.hpp"

using namespace ganim;

void paths()
{
    //auto scene = Scene("paths.mp4", 854, 480, 16, 9, 15);
    auto scene = Scene("paths.mp4", 2560, 1440, 16, 9, 15);
    using namespace vga2;
    auto path = Path(
        {
            -e1,
            e1,
            0.5*e1 + e2,
            4*e1 + e2
        }
    );
    scene.add(path);
    create(scene, path);
    scene.wait(2);
}
