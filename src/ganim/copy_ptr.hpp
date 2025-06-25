#ifndef GANIM_COPY_POINTER_HPP
#define GANIM_COPY_POINTER_HPP

#include <memory>

namespace ganim {
    template <typename T>
    class copy_ptr {
        public:
            copy_ptr()=default;
            copy_ptr(std::unique_ptr<T> ptr) : M_ptr(std::move(ptr)) {}
            ~copy_ptr()=default;
            copy_ptr(copy_ptr&&) noexcept=default;
            copy_ptr& operator=(copy_ptr&&) noexcept=default;
            copy_ptr(const copy_ptr& other)
            {
                M_ptr = std::make_unique<T>(*other.M_ptr);
            }
            copy_ptr& operator=(const copy_ptr& other)
            {
                M_ptr = std::make_unique<T>(*other.M_ptr);
            }
            T* get() noexcept {return M_ptr.get();}
            T* operator->() noexcept {return M_ptr.get();}

        private:
            std::unique_ptr<T> M_ptr = nullptr;
    };
}

#endif
