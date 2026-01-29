#ifndef GANIM_SCRIPT_COMMAND_EXPRESSION_HPP
#define GANIM_SCRIPT_COMMAND_EXPRESSION_HPP

#include "command.hpp"

#include "script/expression/expression.hpp"

namespace ganim::commands {
    class Expression : public Command {
        public:
            Expression(std::unique_ptr<ganim::Expression> expr);
            virtual ExecuteResult execute() const override;

        private:
            std::unique_ptr<ganim::Expression> M_expr;
    };
}

#endif
