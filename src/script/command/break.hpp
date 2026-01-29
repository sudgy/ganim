#ifndef GANIM_SCRIPT_COMMAND_BREAK_HPP
#define GANIM_SCRIPT_COMMAND_BREAK_HPP

#include "command.hpp"

namespace ganim::commands {
    class Break : public Command {
        public:
            virtual ExecuteResult execute() const override;
    };
}

#endif
