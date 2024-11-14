#ifndef GANIM_OBJECT_PARALLELOGRAM_BIVECTOR_HPP
#define GANIM_OBJECT_PARALLELOGRAM_BIVECTOR_HPP

#include "bivector.hpp"

#include "bases/group.hpp"

namespace ganim {
    class ParallelogramBivector : public Group, public Bivector {
        public:
            ParallelogramBivector(
                pointlike auto p1,
                pointlike auto p2,
                pointlike auto p3
            );
    };
}

#endif
