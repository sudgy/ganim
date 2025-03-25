#ifndef GANIM_SCRIPT_SCRIPT_HPP
#define GANIM_SCRIPT_SCRIPT_HPP

#include "script/command/command.hpp"
#include "script/tokenize.hpp"

namespace ganim {
    class Script {
        public:
            explicit Script(std::string script);
            void compile();
            void execute() const;
            const Token& consume_token();
            void expect_semicolon();

        private:
            int M_index = 0;
            std::string M_script;
            std::vector<Token> M_tokens;
            std::vector<std::unique_ptr<Command>> M_commands;
    };
}

#endif
