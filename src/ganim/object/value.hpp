#ifndef GANIM_OBJECT_VALUE_HPP
#define GANIM_OBJECT_VALUE_HPP

/** @file
 * @brief contains the @ref ganim::Value "Value" class.
 */

#include <memory>

#include "animatable.hpp"

namespace ganim {

/** @brief Represents an animatable value of any type
 *
 * This is similar to manim's `ValueTracker`.  However, this class is actually
 * templated, and can be used for any type!  However, to interpolate (and hence
 * animate) it, it must be able to be added and scaled by a `double` value.  It
 * tries to allow for implicit conversions to and from the target type as much
 * as possible, allowing you to use it in many situations as if it was the
 * target type.
 *
 * @tparam T The type that this value will represent.  It defaults to `double`,
 * so if you want that you don't need to specify this template parameter.
 */
template <typename T = double>
class Value : public Animatable {
    public:
        /** @brief Used internally for default initialization. */
        static constexpr bool zero_default = requires(T val) {val = 0;};
        /** @brief Default constructor when the value can be zero initialized */
        Value() requires(zero_default) : M_value(0) {}
        /** @brief Default constructor when the value can't be zero initialized
         *
         * Instead this default initializes the value itself.
         */
        Value() requires(!zero_default) {}
        /** @brief Constructor with a given value */
        Value(T value) : M_value(value) {}
        /** @brief Set the value this represents */
        void set_value(T value) {M_value = value;}

        /** @brief Get the value this represents */
        T get_value() const {return M_value;}
        /** @brief Get the value this represents */
        T operator()() const {return M_value;}
        /** @brief Get the value this represents */
        operator T() const {return M_value;}

        /** @brief Copy the object for the sake of transformations */
        std::unique_ptr<Value> anim_copy() const
            {return std::make_unique<Value<T>>(*this);}
        /** @brief Interpolate between two Values
         *
         * This requires the value type to be addable and scalable.
         */
        void interpolate(const Value& start, const Value& end, double t)
        {
            M_value = (1 - t) * start.M_value + t * end.M_value;
        }

    private:
        T M_value;
};

}

#endif
