#ifndef GANIM_SCRIPT_COMMAND_CONTINUE_HPP
#define GANIM_SCRIPT_COMMAND_CONTINUE_HPP

#include "command.hpp"

namespace ganim::commands {
    class Continue : public Command {
        public:
            virtual ExecuteResult execute() const override;
    };
}

#endif
