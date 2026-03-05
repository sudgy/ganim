#include "compiler_state.hpp"

#include "script/bytecode/bytecodes.hpp"

using namespace ganim;
using namespace ganim::bytecode;

void CompilerState::write_parameter(byte value)
{
    if (value < byte(128)) {
        bytecode.push_back(value);
    }
    else {
        bytecode.push_back(param_byte1);
        bytecode.push_back(value);
    }
}

void CompilerState::write_parameter(int64_t value)
{
    auto bytes = reinterpret_cast<byte*>(&value);
    if (0LL <= value and value < 128LL) {
        bytecode.push_back(bytes[0]);
    }
    else if (-128LL <= value and value < 0LL) {
        bytecode.push_back(param_byte1);
        bytecode.push_back(bytes[0]);
    }
    else if (-0x8000LL <= value and value < 0x8000LL) {
        bytecode.push_back(param_byte2);
        bytecode.push_back(bytes[0]);
        bytecode.push_back(bytes[1]);
    }
    else if (-0x80000000LL <= value and value < 0x80000000LL) {
        bytecode.push_back(param_byte4);
        bytecode.push_back(bytes[0]);
        bytecode.push_back(bytes[1]);
        bytecode.push_back(bytes[2]);
        bytecode.push_back(bytes[3]);
    }
    else {
        bytecode.push_back(param_byte8);
        bytecode.push_back(bytes[0]);
        bytecode.push_back(bytes[1]);
        bytecode.push_back(bytes[2]);
        bytecode.push_back(bytes[3]);
        bytecode.push_back(bytes[4]);
        bytecode.push_back(bytes[5]);
        bytecode.push_back(bytes[6]);
        bytecode.push_back(bytes[7]);
    }
}

void CompilerState::write_parameter(uint64_t value)
{
    auto bytes = reinterpret_cast<byte*>(&value);
    if (value and value < 128ULL) {
        bytecode.push_back(bytes[0]);
    }
    else if (value < 256ULL) {
        bytecode.push_back(param_byte1);
        bytecode.push_back(bytes[0]);
    }
    else if (value < 0x10000ULL) {
        bytecode.push_back(param_byte2);
        bytecode.push_back(bytes[0]);
        bytecode.push_back(bytes[1]);
    }
    else if (value < 0x100000000LL) {
        bytecode.push_back(param_byte4);
        bytecode.push_back(bytes[0]);
        bytecode.push_back(bytes[1]);
        bytecode.push_back(bytes[2]);
        bytecode.push_back(bytes[3]);
    }
    else {
        bytecode.push_back(param_byte8);
        bytecode.push_back(bytes[0]);
        bytecode.push_back(bytes[1]);
        bytecode.push_back(bytes[2]);
        bytecode.push_back(bytes[3]);
        bytecode.push_back(bytes[4]);
        bytecode.push_back(bytes[5]);
        bytecode.push_back(bytes[6]);
        bytecode.push_back(bytes[7]);
    }
}

void CompilerState::write_parameter(double value)
{
    auto bytes = reinterpret_cast<byte*>(&value);
    bytecode.push_back(param_byte8);
    bytecode.push_back(bytes[0]);
    bytecode.push_back(bytes[1]);
    bytecode.push_back(bytes[2]);
    bytecode.push_back(bytes[3]);
    bytecode.push_back(bytes[4]);
    bytecode.push_back(bytes[5]);
    bytecode.push_back(bytes[6]);
    bytecode.push_back(bytes[7]);
}

void CompilerState::resolve_labels()
{
    auto label_map = std::unordered_map<LabelType, uint64_t>();
    for (auto [pos, label] : labels) {
        label_map.emplace(label, pos);
    }
    auto new_bytecode = std::vector<byte>();
    new_bytecode.reserve(bytecode.size());
    uint64_t pos = 0;
    uint64_t bytes_removed = 0;
    int i1 = 0;
    int i2 = 0;
    auto new_label_map = std::unordered_map<LabelType, uint64_t>();
    while (i1 < ssize(labels) or i2 < ssize(jumps)) {
        auto pos1 = i1 < ssize(labels) ? labels[i1].first : -1;
        auto pos2 = i2 < ssize(jumps) ? jumps[i2].first : -1;
        if (pos1 <= pos2) {
            new_label_map[labels[i1].second] = labels[i1].first - bytes_removed;
            ++i1;
        }
        else {
            std::copy(
                bytecode.begin() + pos,
                bytecode.begin() + jumps[i2].first,
                std::back_inserter(new_bytecode)
            );
            auto this_code = bytecode[jumps[i2].first];
            if (this_code == jump_long) pos = jumps[i2].first + 9;
            else pos = jumps[i2].first + 2;
            auto diff = int64_t(label_map[jumps[i2].second]) - int64_t(pos);
            jumps[i2].first = new_bytecode.size();
            if (this_code != jump_long) {
                new_bytecode.push_back(this_code);
                new_bytecode.push_back(byte(0));
            }
            else if (-0x80 <= diff and diff <= 0x7F) {
                new_bytecode.push_back(jump_short);
                new_bytecode.push_back(byte(0));
                bytes_removed += 7;
            }
            else if (-0x8000 <= diff and diff <= 0x7FFF) {
                new_bytecode.push_back(jump_medium);
                new_bytecode.push_back(byte(0));
                new_bytecode.push_back(byte(0));
                bytes_removed += 6;
            }
            else {
                new_bytecode.push_back(jump_long);
                for (int i = 0; i < 8; ++i) new_bytecode.push_back(byte(0));
            }
            ++i2;
        }
    }
    if (pos < bytecode.size()) {
        std::copy(
            bytecode.begin() + pos,
            bytecode.end(),
            std::back_inserter(new_bytecode)
        );

    }
    for (auto [pos, label] : jumps) {
        auto diff = int64_t(new_label_map[label]) - int64_t(pos);
        if (new_bytecode[pos] == jump_medium) {
            diff -= 3;
            auto bytes = reinterpret_cast<byte*>(&diff);
            new_bytecode[pos+1] = bytes[0];
            new_bytecode[pos+2] = bytes[1];
        }
        else if (new_bytecode[pos] == jump_long) {
            auto bytes = reinterpret_cast<byte*>(&new_label_map[label]);
            for (int i = 0; i < 8; ++i) {
                new_bytecode[pos+1+i] = bytes[i];
            }
        }
        else { // Either jump_short or one of the conditional jumps
            diff -= 2;
            auto bytes = reinterpret_cast<byte*>(&diff);
            new_bytecode[pos+1] = bytes[0];
        }
    }
    bytecode = std::move(new_bytecode);
}

CompilerState::LabelType CompilerState::get_next_label()
{
    return M_label++;
}
