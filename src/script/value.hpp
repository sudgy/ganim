#ifndef GANIM_SCRIPT_VALUE_HPP
#define GANIM_SCRIPT_VALUE_HPP

#include "script/any_pointer.hpp"

namespace ganim {
    class Value {
        public:
            virtual ~Value()=default;

            virtual any_pointer value()=0;
            virtual TypeID type() const=0;
    };
}

#endif
