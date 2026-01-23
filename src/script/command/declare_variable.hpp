#ifndef GANIM_SCRIPT_COMMAND_DECLARE_VARIABLE_HPP
#define GANIM_SCRIPT_COMMAND_DECLARE_VARIABLE_HPP

#include "command.hpp"

#include "script/value.hpp"
#include "script/syntax/statement.hpp"

namespace ganim::commands {
    class DeclareVariable : public Command {
        public:
            DeclareVariable(
                SymbolTable& table,
                const syntax::VarStatement& ast
            );
            virtual void execute() const override;

        private:
            mutable std::move_only_function<void()> M_on_execute;
    };
}

#endif
