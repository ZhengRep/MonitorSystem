#include "CommandLineArgs.h"

CommandLineArgs::CommandLineArgs()
{
}

CommandLineArgs::~CommandLineArgs()
{
}

void CommandLineArgs::getArgVector(std::vector<StringStorage>* out) const
{
  *out = m_args;
}
