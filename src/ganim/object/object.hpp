#ifndef GANIM_OBJECT_HPP
#define GANIM_OBJECT_HPP

/** @file
 * @brief Contains the @ref ganim::Object "Object" class
 */

namespace ganim {
    /** @brief The base class for all objects used in scenes */
    class Object {
        public:
            virtual ~Object()=default;
            /** @brief Draw this object onto the screen */
            virtual void draw()=0;
    };
}

#endif
