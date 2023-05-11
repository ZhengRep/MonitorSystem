#include "FileNotFoundException.h"

FileNotFoundException::FileNotFoundException()
  : IOException()
{
}

FileNotFoundException::FileNotFoundException(const TCHAR* message)
  : IOException(message)
{
}

FileNotFoundException::~FileNotFoundException()
{
}
