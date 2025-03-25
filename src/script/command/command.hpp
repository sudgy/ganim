#ifndef GANIM_SCRIPT_COMMAND_BASE_HPP
#define GANIM_SCRIPT_COMMAND_BASE_HPP

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

namespace ganim {
    class Script;
    template <typename Derived> class CommandFactoryBase;
    class Command {
        template <typename Derived> friend class CommandFactoryBase;
        public:
            Command()=default;
            virtual ~Command()=default;

            virtual void execute() const=0;
    };

    inline std::unordered_map<
        std::string,
        std::function<
            std::unique_ptr<Command>(Script& script)
        >
    > G_command_factory;

    template <typename Derived>
    char register_command()
    {
        G_command_factory[Derived::command_name]
            = [](Script& script)
            {return std::make_unique<Derived>(script);};
        return 0;
    }

    template <typename Derived>
    class CommandFactoryBase : public Command {
        inline static char factory_registration = register_command<Derived>();
        friend Derived;
        CommandFactoryBase()
        {
            (void)factory_registration;
        }
    };
}

#endif
