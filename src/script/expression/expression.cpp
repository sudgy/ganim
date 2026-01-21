#include "expression.hpp"

#include "constant.hpp"
#include "variable.hpp"
#include "function.hpp"

#include "script/script.hpp"
#include "script/script_exception.hpp"

using namespace ganim;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}
std::unique_ptr<Expression> Expression::from_ast(
    Script& script,
    const syntax::Expression& ast
)
{
    return expressions::Function::from_ast(script, ast);
}

std::unique_ptr<Expression> Expression::from_ast(
    Script& script,
    const syntax::Factor& ast
)
{
    return std::visit(overloaded{
        [&](const syntax::Constant& ast) -> std::unique_ptr<Expression>
        {
            return expressions::constant_from_ast(ast);
        },
        [&](const syntax::Identifier& ast) -> std::unique_ptr<Expression>
        {
            return expressions::Variable::from_ast(script, ast);
        },
        [&](const syntax::Function& ast) -> std::unique_ptr<Expression>
        {
            return expressions::Function::from_ast(script, ast);
        },
        [&](const std::unique_ptr<syntax::Factor>& factor)
            -> std::unique_ptr<Expression>
        {
            auto fs = [&] -> std::vector<ganim::Function*> {
                if (ast.plus_sign) {
                    return script.get_functions("__unary_plus__");
                }
                else {
                    return script.get_functions("__unary_minus__");
                }
            }();
            auto fact = from_ast(script, *factor);
            auto line_number = fact->line_number();
            auto column_number = fact->column_number();
            for (auto f : fs) {
                if (f->get_input_types()[0] == fact->type()) {
                    auto inputs = std::vector<std::unique_ptr<Expression>>();
                    inputs.emplace_back(std::move(fact));
                    return std::make_unique<expressions::Function>(
                        *f,
                        std::move(inputs),
                        line_number,
                        column_number
                    );
                }
            }
            throw CompileError(line_number, column_number,
                "Unable to find this operation for this type");
        },
        [&](const std::unique_ptr<syntax::Expression>& ast)
            -> std::unique_ptr<Expression>
        {
            return expressions::Function::from_ast(script, *ast);
        }
    }, ast.value);
}
