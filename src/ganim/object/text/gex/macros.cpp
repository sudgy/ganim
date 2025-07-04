#include <vector>
#include <string>

namespace {

std::vector<std::string_view> macro_input = {
R"(\def\vec#1{\mathaccent →{#1}})"
R"(\def\bvec#1{\mathaccent ⇒{#1}})"
R"(\def\tvec#1{\mathaccent ⇛{#1}})"
};

}
