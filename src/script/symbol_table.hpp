#ifndef GANIM_SCRIPT_SYMBOL_TABLE_HPP
#define GANIM_SCRIPT_SYMBOL_TABLE_HPP

#include <unordered_map>

#include "script/function/function.hpp"
#include "script/syntax/type.hpp"

namespace ganim {
    class SymbolTable {
        public:
            SymbolTable();
            void add_builtins();
            void add_variable(
                std::string_view name,
                std::unique_ptr<Value> variable,
                int line_number = -1,
                int column_number = -1
            );
            void add_function(
                std::string_view name,
                std::unique_ptr<Function> function,
                int line_number = -1,
                int column_number = -1
            );
            Type get_type(const syntax::Type& type) const;
            Value* get_variable(const std::string& name) const;
            std::vector<Function*> get_functions(const std::string& name) const;
            void push(SymbolTable& table);
            void pop();

        private:
            std::unordered_map<std::string, std::unique_ptr<Value>>
                M_variables;
            std::unordered_map<std::string,
                std::vector<std::unique_ptr<Function>>> M_functions;
            std::vector<SymbolTable*> M_stack;
    };
}

#endif
