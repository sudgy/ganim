#include "type.hpp"

#include "any_pointer.hpp"
#include "overloaded.hpp"

using namespace ganim;

bool ganim::operator==(const Type& lhs, const Type& rhs)
{
    return std::visit(overloaded{
        [&](const TypeID& lhs_val)
        {
            if (auto rhs_val = get_if<TypeID>(&rhs.value)) {
                return lhs_val == *rhs_val;
            }
            return false;
        },
        [&](const FunctionType* lhs_val)
        {
            if (auto rhs_val = get_if<const FunctionType*>(&rhs.value)) {
                return lhs_val == *rhs_val;
            }
            return false;
        },
        [&](const CustomType* lhs_val)
        {
            if (auto rhs_val = get_if<const CustomType*>(&rhs.value)) {
                return lhs_val == *rhs_val;
            }
            return false;
        }
    }, lhs.value);
}

std::uint64_t Type::size() const
{
    return std::visit(overloaded{
        [](TypeID type) -> std::uint64_t {
            if (type == TypeID(nullptr)) return 0;
            else if (type == any_pointer::get_tag<std::int64_t>()) return 8;
            else if (type == any_pointer::get_tag<double>()) return 8;
            else if (type == any_pointer::get_tag<bool>()) return 1;
            else {
                throw std::runtime_error("The size of this type is not known");
            }
        },
        [](const auto*) -> std::uint64_t {
            throw std::runtime_error("The size of this type is not known");
        }
    }, value);
}
