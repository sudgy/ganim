#ifndef GANIM_SCRIPT_COMMAND_PRINT_HPP
#define GANIM_SCRIPT_COMMAND_PRINT_HPP

#include "command.hpp"

#include "script/expression/expression.hpp"
#include "script/syntax/grammar_types.hpp"

namespace ganim::commands {
    class Print : public Command {
        public:
            Print(Script& script, const syntax::PrintStatement& ast);
            virtual void execute() const override;

            static void set_print_function(
                    std::function<void(std::string_view)>);

        private:
            std::unique_ptr<Expression> M_expr;

            static std::function<void(std::string_view)> S_print_function;
    };
}

#endif
