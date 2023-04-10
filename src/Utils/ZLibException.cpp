#include "ZLibException.h"

ZLibException::ZLibException(const TCHAR* message)
  :Exception(message)
{
}

ZLibException::ZLibException()
{
}

ZLibException::~ZLibException()
{
}
