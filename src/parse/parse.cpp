#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <variant>
#include <format>

//*Grammar
//*LR(0) CLOSURE [Figure 4.32]
//*LR(0) GOTO [Page 246]
//*Grammar -> LR(0) sets [Figure 4.33]
//*FIRST (Page 221)
//*LR(1) CLOSURE [Figure 4.40]
//*LR(1) GOTO [Figure 4.40]
//*LR(0) sets -> LR(0) kernels [Definition on page 245]
//*LR(0) kernels -> Lookaheads [Algorithm 4.62]
//*Grammar -> LALR(1) Kernels [Algorithm 4.63]
//*LALR(1) Sets -> Parsing Tables [Algorithm 4.56]
//*Grammar -> Parsing Tables [Page 270]
// Actual parsing program [Figure 4.36]

namespace {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}

enum class SymbolID : int {
    Unused = -2,
    End = -1,
    Start = 0
};
auto next_symbol_id = SymbolID(1);

auto symbol_names = std::unordered_map<SymbolID, std::string> {
    {SymbolID::Unused, "Unused"},
    {SymbolID::Start, "Start"},
    {SymbolID::End, "End"}
};

std::unordered_map<std::string, SymbolID> symbol_map;
std::vector<std::string> builtin_symbols;

SymbolID add_symbol(const std::string& name)
{
    auto it = symbol_map.find(name);
    if (it != symbol_map.end()) {
        return it->second;
    }
    auto result = next_symbol_id;
    symbol_names[result] = name;
    symbol_map[name] = result;
    next_symbol_id = SymbolID(int(next_symbol_id) + 1);
    return result;
}

struct Symbol {
    SymbolID identifier;
    bool is_terminal = false;
    bool operator==(const Symbol&) const noexcept=default;
};
std::ostream& operator<<(std::ostream& os, const Symbol& symbol)
{
    os << symbol_names[symbol.identifier];
    return os;
}

struct Production {
    SymbolID name;
    std::vector<Symbol> symbols;
    std::string code;
};
std::ostream& operator<<(std::ostream& os, const Production& production)
{
    os << "Production for " << symbol_names[production.name];
    os << ", inputs are ";
    const char* sep = "";
    for (auto& symbol : production.symbols) {
        os << sep << symbol;
        sep = ", ";
    }
    return os;
}

struct Nonterminal {
    SymbolID name;
    std::vector<Production> productions;
    std::string type;
};
std::ostream& operator<<(std::ostream& os, const Nonterminal& nonterminal)
{
    os << "Nonterminal " << symbol_names[nonterminal.name]
       << ".\nProductions:\n";
    for (auto& prod : nonterminal.productions) {
        const char* sep = "    ";
        for (auto& symbol : prod.symbols) {
            os << sep << symbol;
            sep = ", ";
        }
        os << "\n";
    }
    return os;
}

struct Grammar {
    std::unordered_map<SymbolID, Nonterminal> nonterminals;
    std::vector<Symbol> symbols;
    std::unordered_map<SymbolID, std::unordered_set<SymbolID>> first;
};

struct ItemLR0 {
    Production* production = nullptr;
    int index = 0;
    bool operator==(const ItemLR0&) const noexcept=default;
};
std::ostream& operator<<(std::ostream& os, const ItemLR0& item)
{
    os << *item.production << " (index " << item.index << ")";
    return os;
}
template <>
struct std::hash<ItemLR0> {
    std::size_t operator()(const ItemLR0& i) const noexcept
    {
        return std::hash<Production*>()(i.production)
             ^ std::hash<int>()(i.index);
    }
};
using LR0Set = std::unordered_set<ItemLR0>;

struct ItemLR1 {
    ItemLR1()=default;
    ItemLR1(const ItemLR0& item, SymbolID lookahead)
        : production(item.production),
          index(item.index),
          lookahead(lookahead) {}
    ItemLR1(Production* production, int index, SymbolID lookahead)
        : production(production),
          index(index),
          lookahead(lookahead) {}
    Production* production = nullptr;
    int index = 0;
    SymbolID lookahead;
    bool operator==(const ItemLR1&) const noexcept=default;
};
std::ostream& operator<<(std::ostream& os, const ItemLR1& item)
{
    os << *item.production << ", " << "Lookahead: "
       << symbol_names[item.lookahead] << " (index " << item.index << ")";
    return os;
}
template <>
struct std::hash<ItemLR1> {
    std::size_t operator()(const ItemLR1& i) const noexcept
    {
        return std::hash<Production*>()(i.production)
             ^ std::hash<int>()(i.index)
             ^ std::hash<int>()(int(i.lookahead));
    }
};
using LR1Set = std::unordered_set<ItemLR1>;

std::string namespace_name;
std::string class_name;
std::string header_guard;
std::vector<std::string> includes;

std::unordered_map<SymbolID, std::string> tokens;

Grammar grammar;

std::unordered_set<SymbolID> string_first(
    const std::vector<SymbolID>& symbols
)
{
    auto result = std::unordered_set<SymbolID>();
    bool add_empty = true;
    for (auto& symbol : symbols) {
        auto& new_first = grammar.first[symbol];
        result.insert_range(new_first);
        if (!new_first.contains(SymbolID::End)) {
            add_empty = false;
            break;
        }
    }
    if (add_empty) result.insert(SymbolID::End);
    return result;
}

LR0Set closure_lr0(LR0Set result)
{
    auto added = std::unordered_set<SymbolID>();
    bool something_changed = true;
    while (something_changed) {
        something_changed = false;
        for (auto& item : result) {
            if (item.index < ssize(item.production->symbols)) {
                auto symbol = item.production->symbols[item.index];
                if (!symbol.is_terminal) {
                    if (added.insert(symbol.identifier).second) {
                        something_changed = true;
                        auto& new_productions = grammar
                            .nonterminals[symbol.identifier].productions;
                        for (auto& production : new_productions) {
                            result.emplace(&production, 0);
                        }
                        break;
                    }
                }
            }
        }
    }
    return result;
}

LR0Set goto_lr0(const LR0Set& items, const Symbol& X
)
{
    auto base_result = LR0Set();
    for (auto& item : items) {
        if (item.index < ssize(item.production->symbols)) {
            if (item.production->symbols[item.index] == X) {
                auto new_item = item;
                ++new_item.index;
                base_result.insert(std::move(new_item));
            }
        }
    }
    return closure_lr0(base_result);
}

std::vector<LR0Set> sets_lr0()
{
    auto result = std::vector<std::unique_ptr<LR0Set>>();
    result.push_back(std::make_unique<LR0Set>(closure_lr0({{
        &grammar.nonterminals.at(SymbolID::Start).productions[0],
        0
    }})));
    auto added = std::unordered_set<LR0Set*>();
    bool something_changed = true;
    while (something_changed) {
        something_changed = false;
        for (auto& set_pointer : result) {
            if (added.insert(set_pointer.get()).second) {
                auto& set = *set_pointer;
                for (auto& symbol : grammar.symbols) {
                    auto new_set = goto_lr0(set, symbol);
                    if (new_set.empty()) continue;
                    bool already_in = false;
                    for (auto& old_set : result) {
                        if (*old_set == new_set) {
                            already_in = true;
                            break;
                        }
                    }
                    if (!already_in) {
                        result.push_back(
                            std::make_unique<LR0Set>(std::move(new_set))
                        );
                        something_changed = true;
                    }
                }
                if (something_changed) break;
            }
        }
    }
    auto vector_result = std::vector<LR0Set>();
    vector_result.reserve(result.size());
    for (auto& set : result) {
        vector_result.push_back(std::move(*set));
    }
    return vector_result;
}

LR0Set kernel_lr0(LR0Set result)
{
    for (auto it = result.begin(); it != result.end();) {
        auto& item = *it;
        if (item.index == 0) {
            if (item.production->name != SymbolID::Start) {
                it = result.erase(it);
                continue;
            }
        }
        ++it;
    }
    return result;
}

LR1Set closure_lr1(LR1Set result)
{
    bool something_changed = true;
    while (something_changed) {
        something_changed = false;
        for (auto& item : result) {
            if (item.index < ssize(item.production->symbols)) {
                auto& symbols = item.production->symbols;
                auto& symbol = symbols[item.index];
                if (!symbol.is_terminal) {
                    auto& new_productions = grammar
                        .nonterminals[symbol.identifier].productions;
                    for (auto& production : new_productions) {
                        auto follows = std::vector<SymbolID>();
                        for (auto i=item.index+1; i < ssize(symbols); ++i) {
                            follows.push_back(symbols[i].identifier);
                        }
                        if (item.lookahead != SymbolID::End) {
                            follows.push_back(item.lookahead);
                        }
                        if (follows.empty()) {
                            if (result.emplace(&production, 0, SymbolID::End)
                                    .second)
                            {
                                something_changed = true;
                            }
                        }
                        else {
                            for (auto& follow : string_first(follows)) {
                                if (result.emplace(&production, 0, follow)
                                        .second)
                                {
                                    something_changed = true;
                                }
                            }
                        }
                    }
                    if (something_changed) break;
                }
            }
        }
    }
    return result;
}

LR1Set goto_lr1(const LR1Set& items, const Symbol& X
)
{
    auto base_result = LR1Set();
    for (auto& item : items) {
        if (item.index < ssize(item.production->symbols)) {
            if (item.production->symbols[item.index] == X) {
                auto new_item = item;
                ++new_item.index;
                base_result.insert(std::move(new_item));
            }
        }
    }
    return closure_lr1(base_result);
}

std::pair<
    std::vector<std::unordered_map<SymbolID, int>>,
    std::vector<LR1Set>
>
lalr1_kernels()
{
    struct Item {
        ItemLR0 item;
        std::unordered_set<SymbolID> lookaheads;
        std::vector<Item*> propogate_to;
    };
    struct set_info {
        LR0Set set;
        std::vector<Item> kernel;
    };

    auto lr0_sets = sets_lr0();
    auto sets = std::vector<set_info>();
    auto gotos = std::vector<std::unordered_map<SymbolID, int>>();
    sets.reserve(lr0_sets.size());
    gotos.resize(lr0_sets.size());
    for (auto& set : lr0_sets) {
        auto& new_set = sets.emplace_back();
        new_set.set = set;
        auto kernel = kernel_lr0(set);
        new_set.kernel.reserve(kernel.size());
        for (auto& item : kernel) {
            new_set.kernel.push_back({item, {}, {}});
        }
    }

    for (auto i = 0; i < ssize(sets); ++i) {
        for (auto& symbol : grammar.symbols) {
            auto this_goto = goto_lr0(sets[i].set, symbol);
            for (int j = 0; j < ssize(sets); ++j) {
                if (this_goto == sets[j].set) {
                    gotos[i][symbol.identifier] = j;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < ssize(sets); ++i) {
        auto& set = sets[i];
        for (auto& item : set.kernel) {
            auto closure = closure_lr1({ItemLR1(item.item, SymbolID::Unused)});
            for (auto& item1 : closure) {
                if (item1.index < ssize(item1.production->symbols)) {
                    auto& symbol
                        = item1.production->symbols[item1.index].identifier;
                    auto& set1 = sets[gotos[i][symbol]];
                    auto item10 = ItemLR0(item1.production, item1.index + 1);
                    auto& item0 = [&] -> Item& {
                        for (auto& item : set1.kernel) {
                            if (item.item == item10) return item;
                        }
                        throw std::runtime_error("Item doesn't exist?");
                    }();
                    if (item1.lookahead == SymbolID::Unused) {
                        item.propogate_to.push_back(&item0);
                    }
                    else {
                        item0.lookaheads.insert(item1.lookahead);
                    }
                }
            }
        }
    }
    sets[0].kernel[0].lookaheads.insert(SymbolID::End);

    bool something_changed = true;
    while (something_changed) {
        something_changed = false;
        for (auto& set : sets) {
            for (auto& item : set.kernel) {
                for (auto next_item : item.propogate_to) {
                    for (auto& lookahead : item.lookaheads) {
                        if (next_item->lookaheads.insert(lookahead).second) {
                            something_changed = true;
                        }
                    }
                }
            }
        }
    }

    auto result = std::vector<LR1Set>();
    result.reserve(sets.size());
    for (auto& set : sets) {
        auto& new_set = result.emplace_back();
        for (auto& item : set.kernel) {
            for (auto& lookahead : item.lookaheads) {
                new_set.emplace(item.item, lookahead);
            }
        }
    }

    return {gotos, result};
}

void read_grammar_file(std::istream& file)
{
    std::string input;
    while (input != "$grammar") {
        file >> input;
        if (input == "$namespace") {
            file >> namespace_name;
        }
        else if (input == "$class_name") {
            file >> class_name;
        }
        else if (input == "$header_guard") {
            file >> header_guard;
        }
        else if (input == "$include") {
            auto& new_include = includes.emplace_back();
            std::getline(file, new_include);
        }
        else if (input == "$token") {
            std::string new_token;
            file >> new_token;
            auto symbol = add_symbol(new_token);
            auto& new_type = tokens[symbol];
            file >> std::ws;
            std::getline(file, new_type);
            grammar.symbols.emplace_back(symbol, true);
        }
        else if (input == "$nterm") {
            auto new_nterm = std::string();
            file >> new_nterm;
            auto symbol = add_symbol(new_nterm);
            auto& new_nonterm = grammar.nonterminals[symbol];
            file >> std::ws;
            std::getline(file, new_nonterm.type);
        }
    }
    if (namespace_name == "") {
        throw std::runtime_error("Missing namespace name");
    }
    if (class_name == "") {
        throw std::runtime_error("Missing class name");
    }
    if (header_guard == "") {
        throw std::runtime_error("Missing header_guard");
    }
    grammar.symbols.emplace_back(SymbolID::Start, false);
    grammar.symbols.emplace_back(SymbolID::Unused, true);
    grammar.symbols.emplace_back(SymbolID::End, true);
    bool made_start = false;
    while (file >> input) {
        auto new_nonterminal = symbol_map.at(input);
        if (!made_start) {
            made_start = true;
            grammar.nonterminals[SymbolID::Start] = {
                SymbolID::Start,
                {Production(
                    SymbolID::Start,
                    {Symbol(new_nonterminal, false)},
                    "$0 = std::move($1);"
                )},
                grammar.nonterminals[new_nonterminal].type
            };
        }
        file >> input;
        if (input != "=") {
            throw std::runtime_error("Expected = after nonterminal name");
        }
        grammar.symbols.emplace_back(new_nonterminal, false);
        auto& new_nonterm = grammar.nonterminals[new_nonterminal];
        new_nonterm.name = new_nonterminal;
        new_nonterm.productions.emplace_back();
        new_nonterm.productions.back().name = new_nonterminal;
        while (file >> input) {
            auto& prod = new_nonterm.productions.back();
            if (input == ";") break;
            else if (input == "|") {
                new_nonterm.productions.emplace_back();
                new_nonterm.productions.back().name = new_nonterminal;
            }
            else if (input.size() >= 2 and
                input.front() == '"' and input.back() == '"')
            {
                auto str = input.substr(1, input.size() - 2);
                auto token = add_symbol(str);
                prod.symbols.emplace_back(token, true);
                grammar.symbols.emplace_back(token, true);
                builtin_symbols.push_back(str);
            }
            else if (input == "{") {
                auto brace_level = 1;
                file >> std::ws;
                char c;
                while (file.get(c)) {
                    if (c == '{') ++brace_level;
                    if (c == '}') {
                        --brace_level;
                        if (brace_level == 0) {
                            break;
                        }
                    }
                    prod.code.push_back(c);
                }
            }
            else {
                auto it = symbol_map.find(input);
                if (it == symbol_map.end()) {
                    throw std::runtime_error("Unknown symbol " + input);
                }
                auto symbol = it->second;
                prod.symbols.emplace_back(
                    symbol,
                    tokens.contains(symbol)
                );
            }
        }
    }
}

void build_grammar_first()
{
    for (auto& symbol : grammar.symbols) {
        if (symbol.is_terminal) {
            grammar.first[symbol.identifier].insert(symbol.identifier);
        }
    }
    bool something_changed = true;
    while (something_changed) {
        something_changed = false;
        for (auto& symbol : grammar.symbols) {
            if (!symbol.is_terminal) {
                auto& this_first = grammar.first[symbol.identifier];
                for (auto& production : grammar.nonterminals[symbol.identifier]
                    .productions)
                {
                    bool add_empty = true;
                    for (auto& new_symbol : production.symbols) {
                        auto& new_first = grammar.first[new_symbol.identifier];
                        if (&this_first != &new_first) {
                            for (auto& symbol : new_first) {
                                if (this_first.insert(symbol).second) {
                                    something_changed = true;
                                }
                            }
                        }
                        if (!new_first.contains(SymbolID::End)) {
                            add_empty = false;
                            break;
                        }
                    }
                    if (add_empty) {
                        if (this_first.insert(SymbolID::End).second) {
                            something_changed = true;
                        }
                    }
                }
            }
        }
    }
}

struct ActionError {};
struct ActionShift {
    int new_state = -1;
};
struct ActionReduce {
    Production production;
};
struct ActionAccept {
};
using Action = std::variant<
    ActionError,
    ActionShift,
    ActionReduce,
    ActionAccept
>;

struct ParsingTable {
    int state_number = -1;
    std::unordered_map<std::uint64_t, Action> actions;
    std::unordered_map<std::uint64_t, int> gotos;
    static std::uint64_t encode(int state, SymbolID symbol)
    {
        return (std::uint64_t(state) << 32) + std::uint64_t(symbol);
    }
};

ParsingTable lalr1_parsing_table()
{
    auto [gotos, kernels] = lalr1_kernels();
    auto sets = std::vector<LR1Set>();
    sets.reserve(kernels.size());
    for (auto& set : kernels) {
        sets.push_back(closure_lr1(set));
    }

    auto result = ParsingTable();
    result.state_number = ssize(sets);
    auto terminal_gotos = std::unordered_map<std::uint64_t, int>();
    auto state_to_symbol = std::vector<SymbolID>();
    state_to_symbol.resize(sets.size());

    for (int i = 0; i < ssize(sets); ++i) {
        for (auto symbol : grammar.symbols) {
            auto it = gotos[i].find(symbol.identifier);
            if (it != gotos[i].end()) {
                auto j = it->second;
                state_to_symbol[j] = symbol.identifier;
                if (symbol.is_terminal) {
                    terminal_gotos[result.encode(i, symbol.identifier)] = j;
                }
                else {
                    result.gotos[result.encode(i, symbol.identifier)] = j;
                }
            }
        }
        for (auto& item : sets[i]) {
            if (item.index == ssize(item.production->symbols)) {
                if (item.production->name == SymbolID::Start) {
                    if (item.lookahead == SymbolID::End) {
                        result.actions[result.encode(i, SymbolID::End)]
                            = ActionAccept();
                    }
                }
                else {
                    auto& action
                        = result.actions[result.encode(i, item.lookahead)];
                    if (holds_alternative<ActionError>(action)) {
                        action = ActionReduce(*item.production);
                    }
                    else if (holds_alternative<ActionReduce>(action)) {
                        throw std::runtime_error(std::format(
                            "Reduce Reduce conflict with state {} with "
                            "corresponding symbol {}",
                            i,
                            symbol_names[state_to_symbol[i]]));
                    }
                    else if (holds_alternative<ActionShift>(action)) {
                        throw std::runtime_error(std::format(
                            "Shift Reduce conflict with state {} with "
                            "corresponding symbol {}",
                            i,
                            symbol_names[state_to_symbol[i]]));
                    }
                }
            }
            else {
                auto symbol = item.production->symbols[item.index];
                if (symbol.is_terminal) {
                    auto encoded = result.encode(i, symbol.identifier);
                    if (auto it = terminal_gotos.find(encoded);
                        it != terminal_gotos.end())
                    {
                        auto& action = result.actions[encoded];
                        if (holds_alternative<ActionError>(action)) {
                            action = ActionShift(it->second);
                        }
                        else if (holds_alternative<ActionReduce>(action)) {
                            throw std::runtime_error(std::format(
                                "Shift Reduce conflict with state {} with "
                                "corresponding symbol {}",
                                i,
                                symbol_names[state_to_symbol[i]]));
                        }
                    }
                }
            }
        }
    }
    return result;
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        std::cerr << "Usage: parse <grammar file> <skeleton file base>"
                  << " <output filename base>\n";
        return 1;
    }

    auto grammar_file = std::ifstream(argv[1]);
    if (!grammar_file) {
        std::cerr << "Unable to open grammar file \"" << argv[1] << "\"\n";
        return 2;
    }
    read_grammar_file(grammar_file);
    build_grammar_first();
    auto table = lalr1_parsing_table();

    auto skeleton_hpp_name = std::format("{}.hpp", argv[2]);
    auto skeleton_hpp = std::ifstream(skeleton_hpp_name);
    if (!skeleton_hpp) {
        std::cerr << "Unable to open skeleton header \"" << skeleton_hpp_name
                  << "\"\n";
        return 3;
    }
    auto skeleton_cpp_name = std::format("{}.cpp", argv[2]);
    auto skeleton_cpp = std::ifstream(skeleton_cpp_name);
    if (!skeleton_cpp) {
        std::cerr << "Unable to open skeleton file \"" << skeleton_cpp_name
                  << "\"\n";
        return 4;
    }

    auto output_hpp_name = std::format("{}.hpp", argv[3]);
    auto output_hpp = std::ofstream(output_hpp_name);
    if (!output_hpp) {
        std::cerr << "Unable to open output header \"" << output_hpp_name
                  << "\"\n";
        return 5;
    }
    auto output_cpp_name = std::format("{}.cpp", argv[3]);
    auto output_cpp = std::ofstream(output_cpp_name);
    if (!output_cpp) {
        std::cerr << "Unable to open output file \"" << output_cpp_name
                  << "\"\n";
        return 6;
    }

    auto symbol_types = std::unordered_map<SymbolID, std::string>();
    auto token_type = std::string("std::variant<std::monostate");
    auto type_set = std::unordered_set<std::string>{"std::monostate"};
    for (auto& [symbol, token_type_string] : tokens) {
        symbol_types[symbol] = token_type_string;
        if (type_set.insert(token_type_string).second) {
            token_type += ", " + token_type_string;
        }
    }
    auto symbol_type = token_type;
    for (auto& [_, nonterminal] : grammar.nonterminals) {
        symbol_types[nonterminal.name] = nonterminal.type;
        if (type_set.insert(nonterminal.type).second) {
            symbol_type += ", " + nonterminal.type;
        }
    }
    token_type += ">";
    symbol_type += ">";

    auto replace_in_line = [&](std::string& line, std::ofstream& file) {
        auto dollar_pos = 0UZ;
        while (true) {
            dollar_pos = line.find('$', dollar_pos);
            if (dollar_pos == line.npos) break;
            auto line_view = std::string_view(line).substr(dollar_pos);
            if (line_view.starts_with("$HEADER_GUARD")) {
                line.replace(dollar_pos, sizeof("$HEADER_GUARD")-1, header_guard);
            }
            else if (line_view.starts_with("$NAMESPACE")) {
                line.replace(dollar_pos, sizeof("$NAMESPACE")-1, namespace_name);
            }
            else if (line_view.starts_with("$CLASS_NAME")) {
                line.replace(dollar_pos, sizeof("$CLASS_NAME")-1, class_name);
            }
            else if (line_view.starts_with("$TOKEN_TYPE")) {
                line.replace(dollar_pos, sizeof("$TOKEN_TYPE")-1, token_type);
            }
            else if (line_view.starts_with("$RESULT_TYPE")) {
                line.replace(dollar_pos, sizeof("$RESULT_TYPE")-1,
                             grammar.nonterminals[SymbolID::Start].type);
            }
            else if (line_view.starts_with("$SYMBOL_TYPE")) {
                line.replace(dollar_pos, sizeof("$SYMBOL_TYPE")-1, symbol_type);
            }
            else if (line_view.starts_with("$INCLUDES")) {
                line = "";
                for (auto& include : includes) {
                    file << "#include " << include << "\n";
                }
            }
            else if (line_view.starts_with("$TOKEN_FUNC_DECLS")) {
                line = "";
                static constexpr auto sp3 = "            ";
                file << sp3 << "static token_type end_token();\n";
                file << sp3 << "static token_type "
                    "builtin_token(std::string_view token);\n";
                for (auto& [id, type] : tokens) {
                    file << sp3 << "static token_type " << symbol_names.at(id)
                         << "_token(" << type << " value);\n";
                }
            }
            else if (line_view.starts_with("$TOKEN_FUNC_DEFS")) {
                line = "";
                static constexpr auto sp1 = "    ";
                static constexpr auto sp2 = "        ";
                file << class_name << "::token_type "
                     << class_name << "::end_token()\n";
                file << "{\n";
                file << sp1 << "return token_type(-1, std::monostate());\n";
                file << "}\n";
                file << class_name << "::token_type "
                     << class_name
                     << "::builtin_token(std::string_view token)\n";
                file << "{\n";
                for (auto& token : builtin_symbols) {
                    file << sp1 << "if (token == \"";
                    if (token == "\"") file << "\\\"";
                    else file << token;
                    file << "\") {\n";
                    file << sp2 << "return token_type("
                         << int(symbol_map.at(token))
                         << ", std::monostate());\n";
                    file << sp1 << "}\n";
                }
                file << sp1 << "throw std::runtime_error(std::format("
                    "\"Unknown builtin token \\\"{}\\\"\", token));\n";
                file << "}\n";
                for (auto& [id, type] : tokens) {
                    file << class_name << "::token_type "
                         << class_name << "::" << symbol_names.at(id)
                         << "_token(" << type << " value)\n";
                    file << "{\n";
                    file << sp1 << "return token_type(" << int(id)
                         << ", std::move(value));\n";
                    file << "}\n";
                }
            }
            else if (line_view.starts_with("$STATE_ACTION")) {
                line = "";
                static constexpr auto sp2 = "        ";
                static constexpr auto sp3 = "            ";
                for (auto& [encoded, action] : table.actions) {
                    file << sp2 << "case " << encoded << "UL:\n";
                    std::visit(overloaded{
                    [&](ActionError&){},
                    [&](ActionShift& shift) {
                        file << sp3 << "M_states.emplace_back("
                             << shift.new_state << ", "
                             << "convert_to_symbol_type(token.value));\n"
                             << sp3 << "return true;\n";
                    },
                    [&](ActionReduce& reduce) {
                        auto& prod = reduce.production;
                        file << sp2 << "{\n";
                        for (int i = 0; i < ssize(prod.symbols); ++i) {
                            auto it = symbol_types.find(
                                prod.symbols[i].identifier);
                            if (it != symbol_types.end()) {
                                file << sp3 << "[[maybe_unused]] auto& var"
                                     << i + 1 << " = get<" << it->second
                                     << ">(M_states[M_states.size() - "
                                     << ssize(prod.symbols)-i << "].value);\n";
                            }
                        }
                        file << sp3 << "auto var0 = static_cast<"
                             << symbol_types.at(prod.name) << ">(";
                        auto code_view = std::string_view(prod.code);
                        while (true) {
                            auto dollar_pos = code_view.find('$');
                            if (dollar_pos == code_view.npos) {
                                file << code_view << ");\n";
                                break;
                            }
                            file << code_view.substr(0, dollar_pos);
                            code_view.remove_prefix(dollar_pos + 1);
                            auto index = -1;
                            auto result = std::from_chars(
                                code_view.begin(),
                                code_view.end(),
                                index
                            );
                            code_view.remove_prefix(
                                result.ptr - code_view.begin());
                            auto type_id = prod.symbols[index-1].identifier;
                            auto it = symbol_types.find(type_id);
                            if (it == symbol_types.end()) {
                                throw std::runtime_error(std::format(
                                    "Invalid index {} in code {}",
                                    index, prod.code
                                ));
                            }
                            file << "var" << index;
                        }
                        file << sp3 << "M_states.resize(M_states.size() - "
                             << prod.symbols.size() << ");\n";
                        file << sp3 << "auto encoded = (std::uint64_t("
                            "M_states.back().state_id) << 32) + std::uint64_t("
                             << int(prod.name) << ");\n";
                        file << sp3 << "auto new_state = gotos.at(encoded);\n";
                        file << sp3 << "M_states.emplace_back("
                            "new_state, std::move(var0));\n";
                        file << sp3 << "return push(std::move(token));\n";
                        file << sp2 << "}\n";
                    },
                    [&](ActionAccept&) {
                        file << sp3 << "M_finished = true;\n";
                        file << sp3 << "return false;\n";
                    }
                    }, action);
                }
            }
        }
    };

    auto line = std::string();
    while (std::getline(skeleton_hpp, line)) {
        replace_in_line(line, output_hpp);
        output_hpp << line << "\n";
    }

    output_cpp << "#include \"" << output_hpp_name << "\"\n\n";
    output_cpp << "#include <unordered_map>\n";
    output_cpp << "#include <stdexcept>\n";
    output_cpp << "#include <cstdint>\n";
    output_cpp << "#include <format>\n\n";
    output_cpp << "namespace {\n"
               << "    auto gotos = std::unordered_map<std::uint64_t, int>{\n";
    for (auto [encoded, state_id] : table.gotos) {
        output_cpp << "        {" << encoded << ", " << state_id << "},\n";
    }
    output_cpp << "    };\n}\n";
    while (std::getline(skeleton_cpp, line)) {
        replace_in_line(line, output_cpp);
        output_cpp << line << "\n";
    }
}
