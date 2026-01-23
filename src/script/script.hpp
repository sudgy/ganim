#ifndef GANIM_SCRIPT_SCRIPT_HPP
#define GANIM_SCRIPT_SCRIPT_HPP

#include <variant>

#include "script/command/command.hpp"
#include "script/expression/expression.hpp"
#include "script/function/function.hpp"

namespace ganim {
    class Script {
        public:
            explicit Script(std::string script);
            void compile();
            void execute();

            void add_variable(
                std::string_view name,
                std::unique_ptr<Value> variable,
                int line_number = -1,
                int column_number = -1
            );
            void add_function(
                std::string_view name,
                std::unique_ptr<Function> function,
                int line_number = -1,
                int column_number = -1
            );
            Type get_type(const syntax::Type& type);
            Value* get_variable(const std::string& name);
            std::vector<Function*> get_functions(const std::string& name);

        private:
            std::string M_script;
            std::vector<std::unique_ptr<Command>> M_commands;
            std::unordered_map<std::string, std::unique_ptr<Value>>
                M_variables;
            std::unordered_map<std::string,
                std::vector<std::unique_ptr<Function>>> M_functions;
    };
}

#endif
