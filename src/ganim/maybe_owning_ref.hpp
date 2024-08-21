#ifndef GANIM_MAYBE_OWNING_REF_HPP
#define GANIM_MAYBE_OWNING_REF_HPP

#include <utility>
#include <concepts>

namespace ganim {

template <typename T>
class MaybeOwningRef {
    template <typename U>
    friend class MaybeOwningRef;
    public:
        constexpr MaybeOwningRef(T& object)
            : M_object(&object),
              M_owning(false) {}
        constexpr MaybeOwningRef(T&& object)
            : M_object(new T(std::move(object))),
              M_owning(true) {}

        constexpr ~MaybeOwningRef()
        {
            if (M_owning) delete M_object;
        }

        constexpr MaybeOwningRef(MaybeOwningRef&& other)
        :   M_object(other.M_object),
            M_owning(other.M_owning)
        {
            other.M_owning = false;
        }
        template <std::convertible_to<T> U>
        constexpr MaybeOwningRef(MaybeOwningRef<U>&& other)
        :   M_object(other.M_object),
            M_owning(other.M_owning)
        {
            other.M_owning = false;
        }
        constexpr MaybeOwningRef(const MaybeOwningRef& other)=delete;
        constexpr MaybeOwningRef& operator=(MaybeOwningRef&& other)
        {
            if (this != &other) {
                if (M_owning) delete M_object;
                M_object = other.M_object;
                M_owning = other.M_owning;
                other.M_owning = false;
            }
            return *this;
        }
        template <std::convertible_to<T> U>
        constexpr MaybeOwningRef& operator=(MaybeOwningRef<U>&& other)
        {
            // No need to check for this != &other because other has a different
            // type so it can't possibly be the same object.  If it was the same
            // object the normal move constructor would have been called, which
            // does have that check.
            if (M_owning) delete M_object;
            M_object = other.M_object;
            M_owning = other.M_owning;
            other.M_owning = false;
            return *this;
        }
        constexpr MaybeOwningRef& operator=(const MaybeOwningRef& other)=delete;

        constexpr bool is_owning() const {return M_owning;}
        constexpr T& operator*() const {return *M_object;}
        constexpr T* operator->() const {return M_object;}

    private:
        T* M_object;
        bool M_owning;
};

}

#endif
