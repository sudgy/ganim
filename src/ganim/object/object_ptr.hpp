#ifndef GANIM_OBJECT_PTR_HPP
#define GANIM_OBJECT_PTR_HPP

#include <utility>
#include <memory>
#include <stdexcept>
#include <concepts>

namespace ganim {
    /** @brief A shared pointer-like class for managing objects
     *
     * Many things need to own objects: The scene, groups, some animations, the
     * user of the library, etc.  Thus, objects need to be handled using a
     * shared ownership model.  This class is what is used for that.  It has
     * very similar semantics to std::shared_ptr.  It also has a few convenience
     * functions for accessing the object itself, such as forwarding a few
     * operators.  All functions that need to share in the ownership of an
     * object require an ObjectPtr, and since you will almost certainly need to
     * use one of these functions at some point, every object should be made
     * with an ObjectPtr, and not as the object itself.  To make construction
     * easier, every object has a `make_*` function to create ObjectPtrs.
     *
     * ObjectPtrs should be non-null as often as possible.  There are three main
     * ways to get a null ObjectPtr:
     *  - By explicitly constructing it will `nullptr`
     *  - By calling @ref reset
     *  - By getting moved
     */
    template <typename T>
    class ObjectPtr {
        template <typename U>
        friend class ObjectPtr;
        public:
            using element_type = T;

            ObjectPtr(ObjectPtr&& other) noexcept=default;
            ObjectPtr(const ObjectPtr& other)=default;
            ObjectPtr& operator=(ObjectPtr&& other) noexcept=default;
            ObjectPtr& operator=(const ObjectPtr& other)=default;
            /** @brief Explicitly construct a null ObjectPtr. */
            ObjectPtr(std::nullptr_t) {}
            /** @brief Construct an ObjectPtr from an already-existing object.
             *
             * Since the objects pointed to by an ObjectPtr are owned by the
             * ObjectPtr, this will move construct a new object.
             */
            explicit ObjectPtr(T&& object)
                : M_ptr(std::make_shared<T>(std::move(object))) {}
            /** @brief Construct an ObjectPtr using the arguments you would pass
             * to construct the object
             */
            template <typename... Args>
            explicit ObjectPtr(Args&&... args)
                requires(std::constructible_from<T, Args...>)
                : M_ptr(std::make_shared<T>(std::forward<Args>(args)...)) {}
            /** @brief Copy an ObjectPtr of a different but convertible type */
            template <typename U>
            ObjectPtr(const ObjectPtr<U>& other)
                requires(std::convertible_to<U*, T*>)
                : M_ptr(other.M_ptr) {}
            /** @brief Move an ObjectPtr of a different but convertible type */
            template <typename U>
            ObjectPtr(ObjectPtr<U>&& other) noexcept
                requires(std::convertible_to<U*, T*>)
                : M_ptr(std::move(other.M_ptr)) {}
            /** @brief Copy an ObjectPtr of a different but convertible type */
            template <typename U>
            ObjectPtr& operator=(const ObjectPtr<U>& other)
                requires(std::convertible_to<U*, T*>)
            {
                M_ptr = other;
                return *this;
            }
            /** @brief Move an ObjectPtr of a different but convertible type */
            template <typename U>
            ObjectPtr& operator=(ObjectPtr<U>&& other) noexcept
                requires(std::convertible_to<U*, T*>)
            {
                M_ptr = std::move(other.M_ptr);
                return *this;
            }

            /** @brief Create an ObjectPtr from a raw dynamically allocated
             * object, taking ownership.
             *
             * Honestly you shouldn't ever call this.
             */
            static ObjectPtr from_new(T* object)
            {
                if (!object) {
                    throw std::invalid_argument("ObjectPtrs must not be null.");
                }
                return ObjectPtr(object);
            }

            /** @brief Get a raw pointer of the object being managed. */
            T* get() const noexcept {return M_ptr.get();}
            /** @brief Get a reference to the object being managed. */
            T& operator*() const noexcept {return *M_ptr;}
            /** @brief Access the object being managed. */
            T* operator->() const noexcept {return M_ptr.get();}
            /** @brief Get how many ObjectPtrs own this object. */
            int use_count() const noexcept {return M_ptr.use_count();}
            bool operator==(const ObjectPtr&) const noexcept=default;
            bool operator!=(const ObjectPtr&) const noexcept=default;

            /** @brief Explicitly set this ObjectPtr to a null pointer.  This
             * will deallocate the existing object.
             */
            void reset()
            {
                M_ptr.reset();
            }

            /** @brief Copy the underlying object into a new ObjectPtr. */
            ObjectPtr copy_object() const
            {
                return ObjectPtr(T(*M_ptr));
            }

            /** @brief Call dynamic_cast on the underlying ObjectPtr
             *
             * This will create a new ObjectPtr pointing to the same object but
             * with the new type.
             */
            template <typename U>
            ObjectPtr<U> dynamic_pointer_cast() const
            {
                auto new_ptr = std::dynamic_pointer_cast<U>(M_ptr);
                return ObjectPtr<U>(std::move(new_ptr));
            }

            /** @brief A wrapper around the managed object's operator[]
             *
             * Note that std::shared_ptr has this function for the array
             * version, which is different than this function.  ObjectPtr
             * doesn't support arrays.
             */
            auto operator[](int index) const noexcept
                requires(requires(T& object) {object[0];})
                {return (*M_ptr)[index];}
            /** @brief Wrapper around the managed object's iterators */
            auto begin() const noexcept
                requires(requires(T& object) {object.begin();})
                {return M_ptr->begin();}
            /** @brief Wrapper around the managed object's iterators */
            auto end() const noexcept
                requires(requires(T& object) {object.end();})
                {return M_ptr->end();}
            /** @brief Wrapper around the managed object's call operator */
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
