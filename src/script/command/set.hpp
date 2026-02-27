#ifndef GANIM_SCRIPT_COMMAND_SET_HPP
#define GANIM_SCRIPT_COMMAND_SET_HPP

#include "command.hpp"

#include "script/value.hpp"
#include "script/parse/statement.hpp"
#include "script/expression/expression.hpp"

namespace ganim::commands {
    class Set : public Command {
        public:
            Set(SymbolTable& table, const syntax::SetStatement& ast);
            virtual ExecuteResult execute() const override;

        private:
            std::unique_ptr<ganim::Expression> M_lhs;
            std::unique_ptr<ganim::Expression> M_value;
    };
}

#endif
