#ifndef GANIM_SCRIPT_COMPILER_STATE_HPP
#define GANIM_SCRIPT_COMPILER_STATE_HPP

#include "symbol_table.hpp"

namespace ganim {
    using std::byte;
    using std::int64_t;
    using std::uint64_t;
    struct CompilerState {
        std::vector<byte> bytecode;
        void write_parameter(byte value);
        void write_parameter(int64_t value);
        void write_parameter(uint64_t value);
        void write_parameter(double value);

        SymbolTable symbols;

        using LabelType = int;
        std::vector<std::pair<uint64_t, LabelType>> labels;
        std::vector<std::pair<uint64_t, LabelType>> jumps;
        LabelType get_next_label();
        void resolve_labels();

        private:
            int M_label = 0;
    };
}

#endif
