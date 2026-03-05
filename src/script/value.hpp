#ifndef GANIM_SCRIPT_VALUE_HPP
#define GANIM_SCRIPT_VALUE_HPP

#include "script/any_pointer.hpp"
#include "script/type.hpp"

namespace ganim {
    inline namespace old {
    class Value {
        public:
            virtual ~Value()=default;

            virtual any_pointer value()=0;
            virtual Type type() const=0;
            virtual bool modifiable() const {return false;}
            virtual void modify(Value&) {}
    };
    }
}

#endif
