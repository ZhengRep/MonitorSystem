#include "CommandLineFormatException.h"

CommandLineFormatException::CommandLineFormatException(const TCHAR* message)
  : Exception(message)
{
}

CommandLineFormatException::~CommandLineFormatException()
{
}
