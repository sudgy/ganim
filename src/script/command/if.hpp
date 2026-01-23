#ifndef GANIM_SCRIPT_COMMAND_IF_HPP
#define GANIM_SCRIPT_COMMAND_IF_HPP

#include "command.hpp"

#include "script/expression/expression.hpp"

namespace ganim::commands {
    class If : public Command {
        public:
            If(SymbolTable& table, const syntax::IfStatement& ast);
            virtual void execute() const override;

        private:
            std::unique_ptr<Expression> M_condition;
            std::vector<std::unique_ptr<Command>> M_statements;
    };
}

#endif
