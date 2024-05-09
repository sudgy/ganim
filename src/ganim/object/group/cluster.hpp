#ifndef GANIM_OBJECT_CLUSTER_HPP
#define GANIM_OBJECT_CLUSTER_HPP

/** @file
 * @brief Contains the @ref ganim::Cluster "Cluster" class
 */

#include "group_base.hpp"

namespace ganim {

/** @brief The class for a group that is purely organizational
 *
 * This is a group that does not own or hold its subobjects in any way.  It is
 * just used to cluster several objects together to make manipulating several
 * objects at once easier.
 */
class Cluster : public GroupBase {
    public:
        template <typename... Ts>
        explicit Cluster(Ts&... objects)
        {
            (add(objects), ...);
        }
};

}

#endif
