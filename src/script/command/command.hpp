#ifndef GANIM_SCRIPT_COMMAND_BASE_HPP
#define GANIM_SCRIPT_COMMAND_BASE_HPP

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

#include "script/syntax/grammar_types.hpp"

namespace ganim {
    class SymbolTable;
    class Command {
        public:
            Command()=default;
            virtual ~Command()=default;

            static std::unique_ptr<Command>
            from_ast(SymbolTable& table, const syntax::Statement& statement);

            virtual void execute() const=0;
    };
}

#endif
