#include "execute_file.hpp"

#include <fstream>

#include "script/parse/tokenize.hpp"
#include "script/parse/parse.hpp"
#include "script/compile/compiler.hpp"
#include "script/bytecode/interpreter.hpp"

void ganim::execute_file(std::string_view filename)
{
    constexpr auto read_size = std::size_t(4096);
    auto stream = std::ifstream(filename.data());
    stream.exceptions(std::ios_base::badbit);

    if (!stream) {
        throw std::ios_base::failure(
                std::format("Unable to read file {}", filename));
    }

    auto contents = std::string();
    auto buf = std::string(read_size, '\0');
    while (stream.read(&buf[0], read_size)) {
        contents.append(buf, 0, stream.gcount());
    }
    contents.append(buf, 0, stream.gcount());

    auto tokens = tokenize(contents);
    auto ast = parse(tokens);
    auto bytecode = Compiler(ast).take_bytecode();
    auto interp = Interpreter(bytecode);
    interp.execute();
}
