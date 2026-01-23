#ifndef GANIM_SCRIPT_EXPRESSION_HPP
#define GANIM_SCRIPT_EXPRESSION_HPP

#include <memory>

#include "script/value.hpp"

#include "script/syntax/expression.hpp"

namespace ganim {
    class SymbolTable;
    class Expression : public Value {
        public:
            static std::unique_ptr<Expression> from_ast(
                SymbolTable& table,
                const syntax::Expression& ast
            );
            virtual int line_number() const=0;
            virtual int column_number() const=0;
    };
}

#endif
