#ifndef GANIM_ANIMATION_CREATION_HPP
#define GANIM_ANIMATION_CREATION_HPP

/** @file
 * @brief Contains animations related to creating objects.
 */

#include "animation.hpp"

namespace ganim {

class Object;

/** @brief An animation to "create" objects.
 *
 * "Creating" an object is setting its draw fraction continuously from zero to
 * one.  This will automatically make the object visible.
 */
void create(Object& object, AnimationArgs args = AnimationArgs());

}

#endif
