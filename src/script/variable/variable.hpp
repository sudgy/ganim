#ifndef GANIM_SCRIPT_VARIABLE_HPP
#define GANIM_SCRIPT_VARIABLE_HPP

#include <string>
#include <cstdint>

#include "script/type/type.hpp"

namespace ganim {
    class Variable {
        public:
            virtual ~Variable()=default;

            virtual std::string as_string() const=0;
            virtual std::int64_t as_integer() const=0;
            virtual TypeID type() const=0;
    };
}

#endif
