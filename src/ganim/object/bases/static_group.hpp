#ifndef GANIM_OBJECT_STATIC_GROUP_HPP
#define GANIM_OBJECT_STATIC_GROUP_HPP

#include "group.hpp"

namespace ganim {

template <std::derived_from<Object>... Ts>
class StaticGroup : public Group {
    public:
        StaticGroup()
        : M_objects(ObjectPtr<Ts>(nullptr)...) {}
        explicit StaticGroup(ObjectPtr<Ts>... objects)
        : M_objects(objects...)
        {
            add(objects...);
        }
        StaticGroup(const StaticGroup& other)
        :   Group(other, nullptr),
            M_objects(other.copy_impl(std::make_index_sequence<sizeof...(Ts)>{}))
        {
            add_all(std::make_index_sequence<sizeof...(Ts)>{});
        }
        StaticGroup& operator=(const StaticGroup& other)
        {
            Object::operator=(other);
            copy_members(other);
            M_objects = other.copy_impl(std::make_index_sequence<sizeof...(Ts)>{});
            clear();
            add_all(std::make_index_sequence<sizeof...(Ts)>{});
            return *this;
        }
        StaticGroup(StaticGroup&&) noexcept=default;
        StaticGroup& operator=(StaticGroup&&) noexcept=default;
        void set(ObjectPtr<Ts>... objects)
        {
            M_objects = {objects...};
            clear();
            add(objects...);
        }
        template <int i>
        auto get() {return std::get<i>(M_objects);}
        template <int i>
        auto get() const {return std::get<i>(M_objects);}

        ObjectPtr<StaticGroup> polymorphic_copy() const
        {
            return ObjectPtr<StaticGroup>::from_new(polymorphic_copy_impl());
        }

    private:
        std::tuple<ObjectPtr<Ts>...> M_objects;

        virtual StaticGroup* polymorphic_copy_impl() const override
        {
            return new StaticGroup(*this);
        }
        template <std::size_t... Is>
        auto copy_impl(std::index_sequence<Is...>) const
        {
            return std::make_tuple(std::get<Is>(M_objects)->polymorphic_copy()...);
        }
        template <std::size_t... Is>
        auto add_all(std::index_sequence<Is...>)
        {
            (add(std::get<Is>(M_objects)), ...);
        }
};

template <typename... Ts>
auto make_static_group(ObjectPtr<Ts>... objects)
{
    auto result = ObjectPtr<StaticGroup<Ts...>>(objects...);
    return result;
}

}

#endif
