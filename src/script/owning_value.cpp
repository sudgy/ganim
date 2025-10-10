#include "owning_value.hpp"

#include <utility>

using namespace ganim;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}

DelayedOwningValue ganim::make_delayed_owning_value_from(
    std::unique_ptr<Value> value
)
{
    auto type = value->type();
    return std::visit(overloaded{
        [&](const TypeID& type) -> DelayedOwningValue
        {
            if (type == any_pointer::get_tag<std::int64_t>()) {
                auto result = std::make_unique<OwningValue<std::int64_t>>();
                auto result_f = [value = std::move(value), &r = *result] {
                    r.initialize(*value->value().get_as<std::int64_t>());
                };
                return {std::move(result), std::move(result_f)};
            }
            if (type == any_pointer::get_tag<double>()) {
                auto result = std::make_unique<OwningValue<double>>();
                auto result_f = [value = std::move(value), &r = *result] {
                    r.initialize(*value->value().get_as<double>());
                };
                return {std::move(result), std::move(result_f)};
            }
            if (type == any_pointer::get_tag<bool>()) {
                auto result = std::make_unique<OwningValue<bool>>();
                auto result_f = [value = std::move(value), &r = *result] {
                    r.initialize(*value->value().get_as<bool>());
                };
                return {std::move(result), std::move(result_f)};
            }
            if (type == any_pointer::get_tag<std::string>()) {
                auto result = std::make_unique<OwningValue<std::string>>();
                auto result_f = [value = std::move(value), &r = *result] {
                    r.initialize(*value->value().get_as<std::string>());
                };
                return {std::move(result), std::move(result_f)};
            }
            std::unreachable();
        },
        [&](const std::unique_ptr<FunctionType>&) -> DelayedOwningValue
        {
            // TODO: Implement these
            std::unreachable();
        },
        [&](const CustomType*) -> DelayedOwningValue
        {
            // TODO: Implement these
            std::unreachable();
        }
    }, type.value);
}
