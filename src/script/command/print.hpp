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

            static void set_print_function(
                    std::function<void(std::string_view)>);

        private:
            std::unique_ptr<Expression> M_expr;

            static std::function<void(std::string_view)> S_print_function;
    };
}

#endif
