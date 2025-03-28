#ifndef GANIM_SCRIPT_COMMAND_DECLARE_VARIABLE_HPP
#define GANIM_SCRIPT_COMMAND_DECLARE_VARIABLE_HPP

#include "command.hpp"

#include "script/value.hpp"

namespace ganim::commands {
    class DeclareVariable : public CommandFactoryBase<DeclareVariable> {
        public:
            DeclareVariable(Script& script);
            virtual void execute() const override;

            inline static std::string command_name = "var";

        private:
            mutable std::move_only_function<void()> M_on_execute;
    };
}

#endif
