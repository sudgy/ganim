#ifndef GANIM_SCRIPT_EXPRESSION_HPP
#define GANIM_SCRIPT_EXPRESSION_HPP

#include "script/value.hpp"

namespace ganim {
    class Expression : public Value {
        public:
            virtual int line_number() const=0;
            virtual int column_number() const=0;
    };
}

#endif
