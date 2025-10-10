#ifndef GANIM_SCRIPT_TYPE_HPP
#define GANIM_SCRIPT_TYPE_HPP

#include <cstdint>
#include <variant>
#include <vector>
#include <memory>

namespace ganim {
    using TypeID = std::uintptr_t;

    struct FunctionType;
    struct CustomType;

    struct Type {
        std::variant<
            TypeID,
            std::unique_ptr<FunctionType>,
            CustomType*
        > value;
    };

    struct FunctionType {
        Type result_type;
        std::vector<Type> input_types;
        bool operator==(const FunctionType&) const=default;
    };

    struct CustomType {
        // ???
    };

    bool operator==(const Type& lhs, const Type& rhs);
}

#endif
