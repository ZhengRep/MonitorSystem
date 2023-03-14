#include "IOException.h"

IOException::IOException()
{
}

IOException::IOException(const TCHAR* message)
  :Exception(message)
{
}

IOException::~IOException()
{
}
