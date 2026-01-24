#include "symbol_table.hpp"

#include "script_exception.hpp"

#include "function/assignment.hpp"
#include "function/binary.hpp"
#include "function/unary.hpp"
#include "function/print.hpp"
#include "function/equality.hpp"
#include "function/comparison.hpp"
#include "function/boolean.hpp"

using namespace ganim;

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}

SymbolTable::SymbolTable()
{
    add_function("__plus__", std::make_unique<functions::Add<std::int64_t>>());
    add_function("__plus__", std::make_unique<functions::Add<double>>());
    add_function("__plus__", std::make_unique<functions::Add<std::string>>());
    add_function("__minus__", std::make_unique<functions::Subtract<std::int64_t>>());
    add_function("__minus__", std::make_unique<functions::Subtract<double>>());
    add_function("__mult__", std::make_unique<functions::Multiply<std::int64_t>>());
    add_function("__mult__", std::make_unique<functions::Multiply<double>>());
    add_function("__div__", std::make_unique<functions::Divide<std::int64_t>>());
    add_function("__div__", std::make_unique<functions::Divide<double>>());
    add_function("__mod__", std::make_unique<functions::Modulo<std::int64_t>>());

    add_function("__plus_eq__", std::make_unique<functions::AddEq<std::int64_t>>());
    add_function("__plus_eq__", std::make_unique<functions::AddEq<double>>());
    add_function("__plus_eq__", std::make_unique<functions::AddEq<std::string>>());
    add_function("__minus_eq__", std::make_unique<functions::SubtractEq<std::int64_t>>());
    add_function("__minus_eq__", std::make_unique<functions::SubtractEq<double>>());
    add_function("__mult_eq__", std::make_unique<functions::MultiplyEq<std::int64_t>>());
    add_function("__mult_eq__", std::make_unique<functions::MultiplyEq<double>>());
    add_function("__div_eq__", std::make_unique<functions::DivideEq<std::int64_t>>());
    add_function("__div_eq__", std::make_unique<functions::DivideEq<double>>());

    add_function("__unary_plus__",
                 std::make_unique<functions::UnaryPlus<std::int64_t>>());
    add_function("__unary_plus__",
                 std::make_unique<functions::UnaryPlus<double>>());
    add_function("__unary_minus__",
                 std::make_unique<functions::UnaryMinus<std::int64_t>>());
    add_function("__unary_minus__",
                 std::make_unique<functions::UnaryMinus<double>>());

    add_function("__eq__", std::make_unique<functions::Equal<std::int64_t>>());
    add_function("__eq__", std::make_unique<functions::Equal<double>>());
    add_function("__eq__", std::make_unique<functions::Equal<bool>>());
    add_function("__eq__", std::make_unique<functions::Equal<std::string>>());
    add_function("__neq__", std::make_unique<functions::NotEqual<std::int64_t>>());
    add_function("__neq__", std::make_unique<functions::NotEqual<double>>());
    add_function("__neq__", std::make_unique<functions::NotEqual<bool>>());
    add_function("__neq__", std::make_unique<functions::NotEqual<std::string>>());

    add_function("__lt__",
                 std::make_unique<functions::LessThan<std::int64_t>>());
    add_function("__lt__",
                 std::make_unique<functions::LessThan<double>>());
    add_function("__le__",
                 std::make_unique<functions::LessThanOrEqual<std::int64_t>>());
    add_function("__le__",
                 std::make_unique<functions::LessThanOrEqual<double>>());
    add_function("__gt__",
                 std::make_unique<functions::GreaterThan<std::int64_t>>());
    add_function("__gt__",
                 std::make_unique<functions::GreaterThan<double>>());
    add_function("__ge__",
                 std::make_unique<functions::GreaterThanOrEqual<std::int64_t>>());
    add_function("__ge__",
                 std::make_unique<functions::GreaterThanOrEqual<double>>());

    add_function("__and__", std::make_unique<functions::And>());
    add_function("__or__", std::make_unique<functions::Or>());
    add_function("__xor__", std::make_unique<functions::Xor>());
    add_function("__nand__", std::make_unique<functions::Nand>());
    add_function("__nor__", std::make_unique<functions::Nor>());
    add_function("__not__", std::make_unique<functions::Not>());

    add_function("print", std::make_unique<functions::PrintInt>());
    add_function("print", std::make_unique<functions::PrintDouble>());
    add_function("print", std::make_unique<functions::PrintString>());
    add_function("print", std::make_unique<functions::PrintBool>());
}

void SymbolTable::add_variable(
    std::string_view name,
    std::unique_ptr<Value> variable,
    int line_number,
    int column_number
)
{
    auto name_string = std::string(name);
    if (M_variables.contains(name_string)) {
        throw CompileError(line_number, column_number, std::format(
                "A variable by the name \"{}\" already exists.", name));
    }
    M_variables[name_string] = std::move(variable);
}

void SymbolTable::add_function(
    std::string_view name,
    std::unique_ptr<Function> function,
    int line_number,
    int column_number
)
{
    auto name_string = std::string(name);
    auto& functions = M_functions[name_string];
    for (auto& f : functions) {
        if (f->get_result_type() == function->get_result_type() and
            f->get_input_types() == function->get_input_types())
        {
            throw CompileError(line_number, column_number, std::format(
                    "A function by the name \"{}\" already exists.", name));
        }
    }
    functions.emplace_back(std::move(function));
}

Type SymbolTable::get_type(const syntax::Type& type) const
{
    // This function will change later
    if (type.name.name == "int") {
        return Type{any_pointer::get_tag<std::int64_t>()};
    }
    else if (type.name.name == "double") {
        return Type{any_pointer::get_tag<double>()};
    }
    else if (type.name.name == "string") {
        return Type{any_pointer::get_tag<std::string>()};
    }
    else if (type.name.name == "bool") {
        return Type{any_pointer::get_tag<bool>()};
    }
    throw CompileError(type.name.line_number, type.name.column_number,
        std::format("Unknown type \"{}\"", type.name.name));
}

Value* SymbolTable::get_variable(const std::string& name) const
{
    auto it = M_variables.find(name);
    if (it == M_variables.end()) return nullptr;
    else return it->second.get();
}

std::vector<Function*> SymbolTable::get_functions(const std::string& name) const
{
    if (!M_functions.contains(name)) return {};
    auto& results = M_functions.at(name);
    auto result = std::vector<Function*>();
    result.resize(results.size());
    for (int i = 0; i < ssize(results); ++i) {
        result[i] = results[i].get();
    }
    return result;
}
