#include "script.hpp"

using namespace ganim;

Script::Script(std::string script)
: M_script(std::move(script))
{
    M_tokens = tokenize(M_script);
    while (M_index < ssize(M_tokens)) {
        auto token = consume_token();
        auto it = G_command_factory.find(std::string(token.string));
        if (it == G_command_factory.end()) {
            throw std::runtime_error(
                    std::format("Unknown command \"{}\"", token.string));
        }
        M_commands.push_back(it->second(*this));
    }
}

void Script::execute() const
{
    for (auto& command : M_commands) {
        command->execute();
    }
}

const Token& Script::consume_token()
{
    auto& result = M_tokens[M_index];
    ++M_index;
    return result;
}
