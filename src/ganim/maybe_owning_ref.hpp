#ifndef GANIM_MAYBE_OWNING_REF_HPP
#define GANIM_MAYBE_OWNING_REF_HPP

#include <utility>
#include <concepts>

namespace ganim {

/** @brief Represents a reference that possibly owns the object
 *
 * In certain cases, you want to be able to pass both a reference to an external
 * object or a temporary to a particular function, and whatever it is, you want
 * it to outlive that function.  This is a utility class intended to deal with
 * this situation.  If you construct it with an lvalue, it will be a non-owning
 * reference, and if you construct it with an rvalue, it will be an owning
 * reference.  When it goes out of scope, if it's owning, it will destruct the
 * object passed to it in the constructor, otherwise, it will do nothing.
 *
 * Note that unlike most types, this type has well-defined semantics when it has
 * been moved from: Once moved from, a MaybeOwningRef will be pointing at the
 * same object as before, but it will be a non-owning reference.
 *
 * This type is non-copyable, although it's easy to copy a MaybeOwningRef
 * manually with `MaybeOwningRef<T> a = *other`, which creates a new non-owning
 * reference to the object `other` refers to.
 */
template <typename T>
class MaybeOwningRef {
    template <typename U>
    friend class MaybeOwningRef;
    public:
        /** @brief Construct a MaybeOwningRef from an lvalue
         *
         * This will make this be a non-owning reference to `object`.
         */
        constexpr MaybeOwningRef(T& object)
            : M_object(&object),
              M_owning(false) {}
        /** @brief Construct a MaybeOwningRef from an rvalue
         *
         * This will make this be an owning reference to `object`.  Note that it
         * actually moves the input parameter into a newly-constructed object.
         */
        constexpr MaybeOwningRef(T&& object)
            : M_object(new T(std::move(object))),
              M_owning(true) {}
        template <typename U> requires(std::convertible_to<U&, T&>)
        constexpr MaybeOwningRef(U& object)
            : M_object(&object),
              M_owning(false) {}
        template <typename U> requires(std::convertible_to<U&, T&>)
        constexpr MaybeOwningRef(U&& object)
            : M_object(new U(std::move(object))),
              M_owning(true) {}

        constexpr ~MaybeOwningRef()
        {
            if (M_owning) delete M_object;
        }

        /** @brief Move constructor.
         *
         * This object will be the same as other's object.  If other was owning,
         * this will now be owning, and other will not be.
         */
        constexpr MaybeOwningRef(MaybeOwningRef&& other)
        :   M_object(other.M_object),
            M_owning(other.M_owning)
        {
            other.M_owning = false;
        }
        /** @brief Move constructor, allowing for implicit conversions.
         *
         * This object will be the same as other's object.  If other was owning,
         * this will now be owning, and other will not be.
         */
        template <std::convertible_to<T> U>
        constexpr MaybeOwningRef(MaybeOwningRef<U>&& other)
        :   M_object(other.M_object),
            M_owning(other.M_owning)
        {
            other.M_owning = false;
        }
        constexpr MaybeOwningRef(const MaybeOwningRef& other)=delete;
        /** @brief Move assignment.
         *
         * If this was currently owning, it will delete the current object.
         * Then, this object will be the same as other's object.  If other was
         * owning, this will now be owning, and other will not be.
         */
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
        /** @brief Move assignment, allowing for implicit conversions.
         *
         * If this was currently owning, it will delete the current object.
         * Then, this object will be the same as other's object.  If other was
         * owning, this will now be owning, and other will not be.
         */
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

        /** @brief Check if this MaybeOwningRef is an owning reference or not.
         */
        constexpr bool is_owning() const {return M_owning;}
        /** @brief Get the object being referred to. */
        constexpr T& operator*() const {return *M_object;}
        /** @brief Access the object being referred to. */
        constexpr T* operator->() const {return M_object;}

    private:
        T* M_object;
        bool M_owning;
};

}

#endif
