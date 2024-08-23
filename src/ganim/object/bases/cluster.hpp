#ifndef GANIM_OBJECT_CLUSTER_HPP
#define GANIM_OBJECT_CLUSTER_HPP

/** @file
 * @brief Contains the @ref ganim::Cluster "Cluster" class
 */

#include "compound_object.hpp"

namespace ganim {

/** @brief The class for a group that is purely organizational
 *
 * This is a group that does not own or hold its subobjects in any way.  It is
 * just used to cluster several objects together to make manipulating several
 * objects at once easier.
 */
class Cluster : public CompoundObject {
    public:
        template <typename... Ts>
        explicit Cluster(Ts&... objects)
        {
            (add(objects), ...);
        }

        virtual void draw(const Camera& camera) override;
        virtual bool is_visible() const override;
        virtual void draw_outline(const Camera& camera) override;
        virtual void set_outline(const Color& color, double thickness) override;
        virtual void invalidate_outline() override;
        virtual Color get_outline_color() const override;
        virtual double get_outline_thickness() const override;

    private:
        Color M_outline_color = Color("#000000");
        double M_outline_thickness = 0;
};

}

#endif
