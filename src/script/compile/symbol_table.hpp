#ifndef GANIM_SCRIPT_COMPILE_SYMBOL_TABLE_HPP
#define GANIM_SCRIPT_COMPILE_SYMBOL_TABLE_HPP

#include <unordered_map>
#include <optional>

#include "script/parse/type.hpp"
#include "variable.hpp"

namespace ganim {
    class SymbolTable {
        public:
            SymbolTable();
            void add_variable(
                std::string_view name,
                Type type,
                bool modifiable = true,
                int line_number = -1,
                int column_number = -1
            );
            Type get_type(const syntax::Type& type) const;
            std::optional<Variable> get_variable(const std::string& name) const;
            void push(SymbolTable& table);
            uint64_t pop();

        private:
            std::unordered_map<std::string, Variable> M_variables;
            std::size_t M_stack_frame_size = 0;
            std::vector<SymbolTable*> M_stack;
    };
}

#endif
