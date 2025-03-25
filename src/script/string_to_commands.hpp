#ifndef GANIM_SCRIPT_STRING_TO_COMMANDS_HPP
#define GANIM_SCRIPT_STRING_TO_COMMANDS_HPP

#include "script/command/command.hpp"

namespace ganim {
    std::vector<std::unique_ptr<Command>>
    string_to_commands(std::string_view string);
}

#endif
