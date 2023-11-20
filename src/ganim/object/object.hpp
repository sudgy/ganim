#ifndef GANIM_OBJECT_OBJECT_HPP
#define GANIM_OBJECT_OBJECT_HPP

/** @file
 * @brief Contains the @ref ganim::Object "Object" class
 */

#include "transformable.hpp"

namespace ganim {
    /** @brief The base class for objects that have some kind of extent
     */
    class Object : public Transformable {
    };
}

#endif
