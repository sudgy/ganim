#ifndef GANIM_MAYBE_OWNING_REF_HPP
#define GANIM_MAYBE_OWNING_REF_HPP

#include <utility>

namespace ganim {

template <typename T>
class MaybeOwningRef {
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
        constexpr MaybeOwningRef(const MaybeOwningRef& other)
        :   M_object(other.M_object),
            M_owning(false) {}
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
        constexpr MaybeOwningRef& operator=(const MaybeOwningRef& other)
        {
            if (M_owning) delete M_object;
            M_object = other.M_object;
            M_owning = false;
            return *this;
        }

        constexpr bool is_owning() const {return M_owning;}
        constexpr T& operator*() const {return *M_object;}
        constexpr T* operator->() const {return M_object;}

    private:
        T* M_object;
        bool M_owning;
};

}

#endif
