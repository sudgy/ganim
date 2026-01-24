#ifndef $HEADER_GUARD
#define $HEADER_GUARD

#include <vector>
#include <variant>
#include <string>

$INCLUDES

namespace $NAMESPACE {
    class $CLASS_NAME {
        public:
            $CLASS_NAME();

            struct token_type {
                int token_id = -3;
                $TOKEN_TYPE value;
            };
            using result_type = $RESULT_TYPE;
            // True if it ate the token, false otherwise
            bool push(token_type token);
            bool finished() const {return M_finished;}
            const result_type& get_result() const;
            $TOKEN_FUNC_DECLS

        private:
            struct State {
                int state_id = -1;
                $SYMBOL_TYPE value;
            };
            $SYMBOL_TYPE convert_to_symbol_type($TOKEN_TYPE token);
            std::vector<State> M_states;
            bool M_finished = false;
    };
}

#endif
