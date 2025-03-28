#ifndef GANIM_SCRIPT_ANY_POINTER_HPP
#define GANIM_SCRIPT_ANY_POINTER_HPP

#include <cstdint>

namespace ganim {
    using TypeID = std::uintptr_t;
    namespace detail {
        template <typename T>
        struct any_pointer_helper {
            inline static char tag = 0;
        };
    }
    class any_pointer {
        public:
            any_pointer() noexcept=default;
            any_pointer(std::nullptr_t) noexcept {}

            template <typename T>
            any_pointer(T* pointer) noexcept
            :   M_pointer(static_cast<void*>(pointer)),
                M_tag(get_tag<T>()) {}

            any_pointer& operator=(std::nullptr_t) noexcept
            {
                M_pointer = nullptr;
                M_tag = 0;
                return *this;
            }

            template <typename T>
            any_pointer& operator=(T* pointer) noexcept
            {
                M_pointer = static_cast<void*>(pointer);
                M_tag = get_tag<T>();
                return *this;
            }

            template <typename T>
            T* get_as() const noexcept
            {
                if (holds_type<T>()) {
                    return static_cast<T*>(M_pointer);
                }
                return nullptr;
            }

            template <typename T>
            bool holds_type() const noexcept
            {
                return M_tag == get_tag<T>();
            }

            TypeID get_tag() const noexcept {return M_tag;}
            template <typename T>
            static TypeID get_tag() noexcept
            {
                return reinterpret_cast<TypeID>(
                        &detail::any_pointer_helper<T>::tag);
            }

            bool operator==(const any_pointer& other) const noexcept=default;
            bool operator!=(const any_pointer& other) const noexcept=default;

            operator bool() const noexcept
            {
                return M_pointer;
            }

        private:
            void* M_pointer = nullptr;
            TypeID M_tag = 0;
    };
}

#endif
