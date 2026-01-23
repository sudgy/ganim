#ifndef GANIM_SCRIPT_SCRIPT_HPP
#define GANIM_SCRIPT_SCRIPT_HPP

#include "symbol_table.hpp"
#include "script/command/command.hpp"

namespace ganim {
    class Script {
        public:
            explicit Script(std::string script);
            void compile();
            void execute();
            const SymbolTable& symbol_table() const {return M_table;}

        private:
            std::string M_script;
            std::vector<std::unique_ptr<Command>> M_commands;
            SymbolTable M_table;
    };
}

#endif
