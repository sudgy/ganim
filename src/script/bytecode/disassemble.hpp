#ifndef GANIM_SCRIPT_BYTECODE_DISASSEMBLER_HPP
#define GANIM_SCRIPT_BYTECODE_DISASSEMBLER_HPP

#include <iostream>
#include <vector>

namespace ganim {
    // This is included purely for debugging purposes
    void disassemble(const std::vector<std::byte>& code, std::ostream& output);
}

#endif
