#ifndef GANIM_SCRIPT_COMMAND_IF_HPP
#define GANIM_SCRIPT_COMMAND_IF_HPP

#include "command.hpp"

#include "script/expression/expression.hpp"
#include "script/symbol_table.hpp"

namespace ganim::commands {
    class If : public Command {
        public:
            If(SymbolTable& table, const syntax::IfStatement& ast);
            virtual ExecuteResult execute() const override;

        private:
            std::unique_ptr<Expression> M_condition;
            std::vector<std::unique_ptr<Command>> M_true_commands;
            std::vector<std::unique_ptr<Command>> M_false_commands;
            SymbolTable M_true_table;
            SymbolTable M_false_table;
    };
}

#endif
