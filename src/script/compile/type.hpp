#ifndef GANIM_SCRIPT_TYPE_HPP
#define GANIM_SCRIPT_TYPE_HPP

#include <cstdint>
#include <variant>
#include <vector>
#include <memory>

namespace ganim {
    using TypeID = std::uintptr_t;
    namespace detail {
        template <typename T>
        struct type_helper {
            inline static char tag = 0;
        };
    }

    struct FunctionType;
    struct CustomType;

    struct Type {
        Type() : value(TypeID(nullptr)) {}
        std::variant<
            TypeID,
            const FunctionType*,
            const CustomType*
        > value;
        Type(TypeID value) : value(value) {}
        Type(const FunctionType* value) : value(value) {}
        Type(const CustomType* value) : value(value) {}
        Type(std::nullptr_t) : value({TypeID(nullptr)}) {}

        std::uint64_t size() const; // In bytes
        std::uint64_t size8() const; // Number of 8-byte segments needed to fit

        template <typename T>
        static TypeID get_tag() noexcept
        {
            return reinterpret_cast<TypeID>(
                    &detail::type_helper<T>::tag);
        }
    };

    const Type int_type = Type({Type::get_tag<int64_t>()});
    const Type double_type = Type({Type::get_tag<double>()});
    const Type bool_type = Type({Type::get_tag<bool>()});
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
