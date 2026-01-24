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
            const FunctionType*,
            const CustomType*
        > value;
        Type(TypeID value) : value(value) {}
        Type(const FunctionType* value) : value(value) {}
        Type(const CustomType* value) : value(value) {}
        Type(std::nullptr_t) : value({TypeID(nullptr)}) {}
    };

    const Type void_type = Type({TypeID(nullptr)});

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
