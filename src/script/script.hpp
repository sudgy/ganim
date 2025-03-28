#ifndef GANIM_SCRIPT_SCRIPT_HPP
#define GANIM_SCRIPT_SCRIPT_HPP

#include "script/command/command.hpp"
#include "script/expression/expression.hpp"
#include "script/variable.hpp"
#include "script/tokenize.hpp"

namespace ganim {
    class Script {
        public:
            explicit Script(std::string script);
            void compile();
            void execute();
            const Token& get_token();
            const Token& consume_token();
            void expect_semicolon();
            std::unique_ptr<Expression> get_expression();

            void add_variable(
                std::string_view name,
                std::unique_ptr<Value> variable,
                int line_number = -1,
                int column_number = -1
            );
            Value* get_variable(const std::string& name);

        private:
            int M_index = 0;
            std::string M_script;
            std::vector<Token> M_tokens;
            std::vector<std::unique_ptr<Command>> M_commands;
            std::unordered_map<std::string, std::unique_ptr<Value>>
                M_variables;

            struct Piece;
            struct Identifier {
                std::string_view identifier;
            };
            struct Literal {
                std::string_view token;
            };
            struct Operator {
                int precedence = 0;
                std::string_view token;
            };
            struct Compound {
                std::vector<Piece> pieces;
            };
            struct Processed {
                std::unique_ptr<Expression> expression;
            };
            struct Piece {
                int line_number = -1;
                int column_number = -1;
                std::variant<Identifier, Literal, Operator, Compound, Processed>
                    data;
            };

            std::vector<Piece> get_expression_pieces();
            std::unique_ptr<Expression> combine_expression_pieces(
                    std::vector<Piece> pieces);
            void handle_operator_piece(
                    Operator& op, std::vector<Piece>& pieces, int& index);
            std::unique_ptr<Expression> convert_single_piece(Piece& piece);
    };
}

#endif
