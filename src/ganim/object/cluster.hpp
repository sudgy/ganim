#ifndef GANIM_OBJECT_CLUSTER_HPP
#define GANIM_OBJECT_CLUSTER_HPP

#include "group_base.hpp"

namespace ganim {

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
