#include <vector>
#include <string>

namespace {

std::vector<std::string_view> macro_input = {
"\\def\\vec#1{#1\xE2\x83\x97}" // Temporary solution
};

}
