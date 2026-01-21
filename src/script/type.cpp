#include "type.hpp"

using namespace ganim;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}

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
