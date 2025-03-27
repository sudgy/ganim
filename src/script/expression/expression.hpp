#ifndef GANIM_SCRIPT_EXPRESSION_HPP
#define GANIM_SCRIPT_EXPRESSION_HPP

#include <string>
#include <cstdint>

namespace ganim {
    enum class ExpressionType {String, Integer};
    class Expression {
        public:
            virtual ~Expression()=default;

            virtual std::string as_string() const=0;
            virtual std::int64_t as_integer() const=0;
            virtual ExpressionType type() const=0;
    };
}

#endif
