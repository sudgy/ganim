#include "string_to_commands.hpp"

#include "script/command/command.hpp"

using namespace ganim;

std::vector<std::unique_ptr<Command>>
ganim::string_to_commands(std::string_view string)
{
    auto result = std::vector<std::unique_ptr<Command>>();
    auto remove_starting_whitespace = [&]{
        while (!string.empty() and std::isspace(string.front())) {
            string.remove_prefix(1);
        }
    };
    while (!string.empty()) {
        remove_starting_whitespace();
        if (string.empty()) break;
        auto pos = 0uz;
        while (!string.empty() and std::isalnum(string[pos])) {
            ++pos;
        }
        auto command_name = string.substr(0, pos);
        auto it = G_command_factory.find(std::string(command_name));
        if (it == G_command_factory.end()) {
            throw std::runtime_error(
                    std::format("Unknown command \"{}\"", command_name));
        }
        while (pos < string.size() and std::isspace(string[pos])) ++pos;
        auto pos2 = string.find_first_of(';');
        if (pos2 == string.npos) {
            throw std::runtime_error("Expected semicolon");
        }
        auto command = string.substr(pos, pos2 - pos);
        result.push_back(std::unique_ptr<Command>(it->second(string.substr(0, pos2), command)));
        string.remove_prefix(pos2 + 1); // Include the semicolon
    }
    return result;
}
