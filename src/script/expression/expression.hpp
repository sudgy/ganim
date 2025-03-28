#ifndef GANIM_SCRIPT_EXPRESSION_HPP
#define GANIM_SCRIPT_EXPRESSION_HPP

#include <string>
#include <cstdint>

#include "script/any_pointer.hpp"

namespace ganim {
    class Expression {
        public:
            virtual ~Expression()=default;

            virtual any_pointer value()=0;
            virtual TypeID type() const=0;

            virtual int line_number() const=0;
            virtual int column_number() const=0;
    };
}

#endif
