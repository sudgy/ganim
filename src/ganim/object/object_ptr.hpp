#ifndef GANIM_OBJECT_PTR_HPP
#define GANIM_OBJECT_PTR_HPP

#include <utility>
#include <memory>
#include <stdexcept>
#include <concepts>

namespace ganim {
    template <typename T>
    class ObjectPtr {
        template <typename U>
        friend class ObjectPtr;
        public:
            using element_type = T;

            ObjectPtr(std::nullptr_t) {}
            explicit ObjectPtr(T&& object)
                : M_ptr(std::make_shared<T>(std::move(object))) {}
            template <typename... Args>
            explicit ObjectPtr(Args&&... args)
                requires(std::constructible_from<T, Args...>)
                : M_ptr(std::make_shared<T>(std::forward<Args>(args)...)) {}
            template <typename U>
            ObjectPtr(const ObjectPtr<U>& other)
                requires(std::convertible_to<U*, T*>)
                : M_ptr(other.M_ptr) {}
            template <typename U>
            ObjectPtr(ObjectPtr<U>&& other) noexcept
                requires(std::convertible_to<U*, T*>)
                : M_ptr(std::move(other.M_ptr)) {}
            template <typename U>
            ObjectPtr& operator=(const ObjectPtr<U>& other)
                requires(std::convertible_to<U*, T*>)
            {
                M_ptr = other;
                return *this;
            }
            template <typename U>
            ObjectPtr& operator=(ObjectPtr<U>&& other) noexcept
                requires(std::convertible_to<U*, T*>)
            {
                M_ptr = std::move(other.M_ptr);
                return *this;
            }

            static ObjectPtr from_new(T* object)
            {
                if (!object) {
                    throw std::invalid_argument("ObjectPtrs must not be null.");
                }
                return ObjectPtr(object);
            }

            T* get() const noexcept {return M_ptr.get();}
            T& operator*() const noexcept {return *M_ptr;}
            T* operator->() const noexcept {return M_ptr.get();}
            int use_count() const noexcept {return M_ptr.use_count();}
            bool operator==(const ObjectPtr&) const noexcept=default;
            bool operator!=(const ObjectPtr&) const noexcept=default;

            void reset()
            {
                M_ptr.reset();
            }

            ObjectPtr copy_object() const
            {
                return ObjectPtr(T(*M_ptr));
            }

            template <typename U>
            ObjectPtr<U> dynamic_pointer_cast() const
            {
                auto new_ptr = std::dynamic_pointer_cast<U>(M_ptr);
                return ObjectPtr<U>(std::move(new_ptr));
            }

            auto operator[](int index) const noexcept
                requires(requires(T& object) {object[0];})
                {return (*M_ptr)[index];}
            auto begin() const noexcept
                requires(requires(T& object) {object.begin();})
                {return M_ptr->begin();}
            auto end() const noexcept
                requires(requires(T& object) {object.end();})
                {return M_ptr->end();}
            auto operator()() const
                requires(requires(T& object) {object();})
                {return (*M_ptr)();}

        private:
            ObjectPtr(T* object) : M_ptr(object) {}
            ObjectPtr(std::shared_ptr<T> ptr) : M_ptr(std::move(ptr)) {}
            std::shared_ptr<T> M_ptr;
    };
}

#endif
