#include "preprocess.hpp"

#include "preprocessor.hpp"

using namespace ganim;
using namespace ganim::gex;

TokenList ganim::gex::preprocess(
    bool math,
    const std::vector<std::string_view>& input
)
{
    auto preprocessor = Preprocessor(math);
    preprocessor.process(input);
    return preprocessor.get_output();
}
