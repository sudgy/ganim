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
            return polymorphic_copy_impl_impl(
                    std::make_index_sequence<sizeof...(Ts)>{});
        }
        template <std::size_t... Is>
        StaticGroup*
        polymorphic_copy_impl_impl(std::index_sequence<Is...>) const
        {
            return new StaticGroup(
                    std::get<Is>(M_objects)->polymorphic_copy()...);
        }
};

template <typename... Ts>
auto make_static_group(ObjectPtr<Ts>&&... objects)
{
    auto result = ObjectPtr<StaticGroup<Ts...>>(
        std::forward<ObjectPtr<Ts>>(objects)...
    );
    return result;
}

}

#endif
