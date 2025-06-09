#ifndef GANIM_OBJECT_TYPED_GROUP_HPP
#define GANIM_OBJECT_TYPED_GROUP_HPP

#include "group.hpp"

namespace ganim {

template <std::derived_from<Object> T>
class TypedGroup : public Group {
    public:
        TypedGroup()=default;
        TypedGroup(const TypedGroup& other)
        :   Group(other, nullptr)
        {
            for (auto& obj : other.M_subobjects) add(obj->polymorphic_copy());
        }
        TypedGroup& operator=(const TypedGroup& other)
        {
            Object::operator=(other);
            copy_members(other);
            clear();
            for (auto& obj : other.M_subobjects) add(obj->polymorphic_copy());
            return *this;
        }
        TypedGroup(TypedGroup&&) noexcept=default;
        TypedGroup& operator=(TypedGroup&&) noexcept=default;

        void add(ObjectPtr<T> object)
        {
            M_subobjects.push_back(object);
            Group::add(object);
        }
        template <normal_input_range R>
        void add(R& range)
        {
            for (auto& object : range) {
                add(object);
            }
        }
        template <typename... Ts> requires(sizeof...(Ts) > 1)
        void add(Ts&... objects)
        {
            (add(objects), ...);
        }

        void remove(T& object)
        {
            for (auto it = M_subobjects.begin(); it != M_subobjects.end(); ++it)
            {
                if (&**it == &object) {
                    M_subobjects.erase(it);
                    break;
                }
            }
            Group::remove(object);
        }
        inline void remove(ObjectPtr<T> object) {remove(*object);}
        template <normal_input_range R>
        void remove(R& range)
        {
            for (auto& object : range) {
                remove(object);
            }
        }
        template <typename... Ts> requires(sizeof...(Ts) > 1)
        void remove(Ts&... objects)
        {
            (remove(objects), ...);
        }

        ObjectPtr<TypedGroup> polymorphic_copy() const
        {
            return ObjectPtr<TypedGroup>::from_new(polymorphic_copy_impl());
        }

        auto begin() {return M_subobjects.begin();}
        auto end() {return M_subobjects.end();}
        auto begin() const {return M_subobjects.begin();}
        auto end() const {return M_subobjects.end();}
        auto cbegin() const {return M_subobjects.begin();}
        auto cend() const {return M_subobjects.end();}
        int size() const {return static_cast<int>(M_subobjects.size());}
        ObjectPtr<T> operator[](int index)
            {return M_subobjects[index];}
        ObjectPtr<const T> operator[](int index) const
            {return M_subobjects[index];}
        void clear() {M_subobjects.clear(); Group::clear();}

        ObjectPtr<TypedGroup> range(int i1, int i2)
        {
            auto result = ObjectPtr<TypedGroup>();
            if (i1 < 0) i1 = size() + i1;
            if (i2 < 0) i2 = size() + i2;
            if (i1 < 0) {
                throw std::out_of_range(
                        "Starting index too low when finding a range of a group");
            }
            if (i2 > size()) {
                throw std::out_of_range(
                        "Ending index too high when finding a range of a group");
            }
            for (int i = i1; i < i2; ++i) {
                result->add(M_subobjects[i]);
            }
            result->set_draw_subobject_ratio(get_draw_subobject_ratio());
            if (drawing_together()) result->draw_together();
            if (is_fixed_in_frame()) result->set_fixed_in_frame(true);
            if (is_fixed_orientation()) result->set_fixed_orientation(true);
            return result;
        }
        ObjectPtr<TypedGroup> range(int i)
        {
            auto result = ObjectPtr<TypedGroup>();
            if (i < 0) i = size() + i;
            if (i < 0) {
                throw std::out_of_range(
                        "Starting index too low when finding a range of a group");
            }
            if (i >= size()) {
                throw std::out_of_range(
                        "Starting index too high when finding a range of a group");
            }
            for (; i < size(); ++i) {
                result->add(M_subobjects[i]);
            }
            result->set_draw_subobject_ratio(get_draw_subobject_ratio());
            if (drawing_together()) result->draw_together();
            if (is_fixed_in_frame()) result->set_fixed_in_frame(true);
            if (is_fixed_orientation()) result->set_fixed_orientation(true);
            return result;
        }

    private:
        std::vector<ObjectPtr<T>> M_subobjects;

        virtual TypedGroup* polymorphic_copy_impl() const override
        {
            return new TypedGroup(*this);
        }
};

template <std::derived_from<Object> T, std::derived_from<T>... Ts>
auto make_typed_group(ObjectPtr<Ts>... objects)
{
    auto result = ObjectPtr<TypedGroup<T>>();
    (result->add(objects), ...);
    return result;
}

}

#endif
