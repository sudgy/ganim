#ifndef GANIM_SCRIPT_COMMAND_LOOP_HPP
#define GANIM_SCRIPT_COMMAND_LOOP_HPP

#include "command.hpp"

#include "script/expression/expression.hpp"
#include "script/symbol_table.hpp"

namespace ganim::commands {
    class Loop : public Command {
        public:
            Loop(SymbolTable& table, const syntax::LoopStatement& ast);
            virtual ExecuteResult execute() const override;

        private:
            std::vector<std::unique_ptr<Command>> M_loop_commands;
            SymbolTable M_loop_table;
    };
}

#endif
