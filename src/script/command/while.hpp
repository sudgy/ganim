#ifndef GANIM_SCRIPT_COMMAND_WHILE_HPP
#define GANIM_SCRIPT_COMMAND_WHILE_HPP

#include "command.hpp"

#include "script/expression/expression.hpp"
#include "script/symbol_table.hpp"

namespace ganim::commands {
    class While : public Command {
        public:
            While(SymbolTable& table, const syntax::WhileStatement& ast);
            virtual void execute() const override;

        private:
            std::unique_ptr<Expression> M_condition;
            std::vector<std::unique_ptr<Command>> M_loop_commands;
            SymbolTable M_loop_table;
    };
}

#endif
