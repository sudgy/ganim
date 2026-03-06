#ifndef GANIM_SCRIPT_COMPILER_HPP
#define GANIM_SCRIPT_COMPILER_HPP

#include <unordered_map>
#include <optional>

#include "script/parse/type.hpp"
#include "variable.hpp"
#include "script/parse/statement.hpp"

namespace ganim {
    using std::byte;
    using std::int64_t;
    using std::uint64_t;
    class Compiler {
        public:
            Compiler(const std::vector<syntax::Statement>& ast);

            using LabelType = int;
            std::vector<byte> take_bytecode();
            void write_byte(byte value);
            void write_parameter(byte value);
            void write_parameter(int64_t value);
            void write_parameter(uint64_t value);
            void write_parameter(double value);

            void write_pop(uint64_t size);
            void write_jump(LabelType label);
            void write_jump(byte jump_bytecode, LabelType label);

            LabelType get_next_label();
            void resolve_labels();
            void add_label_reference(LabelType label);
            void add_label_reference(std::uint64_t position, LabelType label);

            void add_variable(
                std::string_view name,
                Type type,
                bool modifiable = true,
                int line_number = -1,
                int column_number = -1
            );
            std::optional<Variable> get_variable(const std::string& name) const;
            Type get_type(const syntax::Type& type) const;
            void push_symbols();
            uint64_t pop_symbols();

        private:
            struct SymbolTable {
                std::unordered_map<std::string, Variable> M_variables;
                std::size_t M_stack_frame_size = 0;
            };

            std::vector<byte> M_bytecode;
            std::vector<SymbolTable> M_stack;
            std::vector<std::pair<uint64_t, LabelType>> M_labels;
            std::vector<std::pair<uint64_t, LabelType>> M_jumps;
            int M_label = 0;
    };
}

#endif
