#include "continue.hpp"

using namespace ganim;
using namespace ganim::commands;

Command::ExecuteResult Continue::execute() const
{
    return Command::Continue;
}
