#ifndef GANIM_SCRIPT_FUNCTION_HPP
#define GANIM_SCRIPT_FUNCTION_HPP

#include "script/value.hpp"

namespace ganim {
    class Function {
        public:
            Function(FunctionType type) : M_type(type) {}
            virtual ~Function()=default;

            virtual std::unique_ptr<Value> execute(std::vector<Value*> vs) const=0;

            Type get_result_type() const {return M_type.result_type;}
            const std::vector<Type>& get_input_types() const
                {return M_type.input_types;}

        private:
            FunctionType M_type;
    };
}

#endif
