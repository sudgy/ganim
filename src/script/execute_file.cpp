#include "execute_file.hpp"

#include <fstream>

#include "string_to_commands.hpp"

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

    auto commands = string_to_commands(contents);
    for (auto& command : commands) {
        command->execute();
    }
}
