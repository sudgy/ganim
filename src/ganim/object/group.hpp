#ifndef GANIM_OBJECT_GROUP_HPP
#define GANIM_OBJECT_GROUP_HPP

#include "group_base.hpp"
#include "drawable.hpp"

namespace ganim {

class Group : public GroupBase, public Drawable {
    public:
        template <typename... Ts>
        explicit Group(Ts&... objects)
        {
            (add(objects), ...);
        }

        virtual void add(Object& object) override;

        virtual void draw(const Camera& camera) override;
        virtual bool is_visible() const override;

    private:
        std::vector<Drawable*> M_drawables;
};

}

#endif
