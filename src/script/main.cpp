#include "script/execute_file.hpp"

namespace ganim {
    int script_main(int argc, char* argv[])
    {
        if (argc == 2) {
            execute_file(argv[1]);
        }
        return 0;
    }
}
