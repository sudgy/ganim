#ifndef GANIM_SCRIPT_EXPRESSION_HPP
#define GANIM_SCRIPT_EXPRESSION_HPP

#include <string>

namespace ganim {
    enum class ExpressionType {String};
    class Expression {
        public:
            virtual ~Expression()=default;

            virtual std::string as_string() const=0;
            virtual ExpressionType type() const=0;
    };
}

#endif
