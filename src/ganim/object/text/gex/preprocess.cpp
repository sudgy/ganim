#include "preprocess.hpp"

#include "preprocessor.hpp"

using namespace ganim;
using namespace ganim::gex;

TokenList ganim::gex::preprocess(const std::vector<std::string_view>& input)
{
    auto preprocessor = Preprocessor();
    preprocessor.process(input);
    return preprocessor.get_output();
}
