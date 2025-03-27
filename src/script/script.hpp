#ifndef GANIM_SCRIPT_SCRIPT_HPP
#define GANIM_SCRIPT_SCRIPT_HPP

#include "script/command/command.hpp"
#include "script/expression/expression.hpp"
#include "script/variable/variable.hpp"
#include "script/tokenize.hpp"

namespace ganim {
    class Script {
        public:
            explicit Script(std::string script);
            void compile();
            void execute() const;
            const Token& get_token();
            const Token& consume_token();
            void expect_semicolon();
            std::unique_ptr<Expression> get_expression();

            void add_variable(
                std::string_view name,
                std::unique_ptr<Variable> variable,
                int line_number = -1,
                int column_number = -1
            );
            Variable* get_variable(const std::string& name);

        private:
            int M_index = 0;
            std::string M_script;
            std::vector<Token> M_tokens;
            std::vector<std::unique_ptr<Command>> M_commands;
            std::unordered_map<std::string, std::unique_ptr<Variable>>
                M_variables;
    };
}

#endif
