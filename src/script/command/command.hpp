#ifndef GANIM_SCRIPT_COMMAND_BASE_HPP
#define GANIM_SCRIPT_COMMAND_BASE_HPP

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

namespace ganim {
    class Script;
    class Command {
        public:
            Command()=default;
            virtual ~Command()=default;

            virtual void execute() const=0;
    };
}

#endif
