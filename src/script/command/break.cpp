#include "break.hpp"

using namespace ganim;
using namespace ganim::commands;

Command::ExecuteResult Break::execute() const
{
    return Command::Break;
}
