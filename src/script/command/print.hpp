#ifndef GANIM_SCRIPT_COMMAND_PRINT_HPP
#define GANIM_SCRIPT_COMMAND_PRINT_HPP

#include "command.hpp"

namespace ganim::commands {
    class Print : public CommandFactoryBase<Print> {
        public:
            Print(Script& script);
            virtual void execute() const override;

            inline static std::string command_name = "print";

        private:
            // This will be very different in the future
            std::string M_string;
    };
}

#endif
