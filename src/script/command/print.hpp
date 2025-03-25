#ifndef GANIM_SCRIPT_COMMAND_PRINT_HPP
#define GANIM_SCRIPT_COMMAND_PRINT_HPP

#include "command.hpp"

#include "script/expression/expression.hpp"

namespace ganim::commands {
    class Print : public CommandFactoryBase<Print> {
        public:
            Print(Script& script);
            virtual void execute() const override;

            inline static std::string command_name = "print";

        private:
            std::unique_ptr<Expression> M_string;
    };
}

#endif
