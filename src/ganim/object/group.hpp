#ifndef GANIM_OBJECT_GROUP_HPP
#define GANIM_OBJECT_GROUP_HPP

/** @file
 * @brief Contains the @ref ganim::Group "Group" class
 */

#include "group_base.hpp"
#include "drawable.hpp"

namespace ganim {

/** @brief The class for a group that "holds" all of its subobjects
 *
 * This class is a group class that is considered to "hold" all of its
 * subobjects, at least for the purpose of drawing.  It doesn't actually own its
 * subobjects.  Unless you want this special drawing behavior you probably want
 * to use @ref Cluster instead.
 */
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
