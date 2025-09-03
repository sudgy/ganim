#include <vector>
#include <string>

namespace {

std::vector<std::string_view> macro_input = {
R"(\def\vec#1{\mathaccent →{#1}})",
R"(\def\bvec#1{\mathaccent ⇒{#1}})",
R"(\def\tvec#1{\mathaccent ⇛{#1}})",

R"(\def\over{\abovewithdelims.. -1pt})",
R"(\def\atop{\abovewithdelims.. 0pt})",
R"(\def\choose{\abovewithdelims() 0pt})",
R"(\def\above{\abovewithdelims.. })",
R"(\def\overwithdelims#1#2{\abovewithdelims#1#2 -1pt})",
R"(\def\atopwithdelims#1#2{\abovewithdelims#1#2 0pt})",

R"(\def\,{\mskip 3mu})",
R"(\def\>{\mskip 4mu})",
R"(\def\;{\mskip 5mu})",
R"(\def\!{\mskip -3mu})",

R"(\def\cdots{\mathinner{⋯}})",

R"(\def\sqrt#1{\radical √{#1}})",
};

}
