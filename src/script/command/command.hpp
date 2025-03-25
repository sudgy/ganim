#ifndef GANIM_SCRIPT_COMMAND_BASE_HPP
#define GANIM_SCRIPT_COMMAND_BASE_HPP

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

namespace ganim {
    template <typename Derived> class CommandFactoryBase;
    class Command {
        template <typename Derived> friend class CommandFactoryBase;
        public:
            explicit Command(std::string command_string)
                : M_command_string(command_string) {}
            virtual ~Command()=default;

            virtual void execute() const=0;

        private:
            std::string M_command_string;
    };

    inline std::unordered_map<
        std::string,
        std::function<
            std::unique_ptr<Command>(std::string_view, std::string_view)
        >
    > G_command_factory;

    template <typename Derived>
    char register_command()
    {
        G_command_factory[Derived::command_name]
            = [](std::string_view full_command_string,
                 std::string_view command_string)
            {return std::make_unique<Derived>(
                    full_command_string, command_string);};
        return 0;
    }

    template <typename Derived>
    class CommandFactoryBase : public Command {
        inline static char factory_registration = register_command<Derived>();
        friend Derived;
        CommandFactoryBase(std::string_view command_string)
        : Command(std::string(command_string))
        {
            (void)factory_registration;
        }
    };
}

#endif
