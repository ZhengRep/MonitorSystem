#include "FileTransferException.h"

FileTransferException::FileTransferException()
  : Exception()
{
}

FileTransferException::FileTransferException(const TCHAR* message)
  : Exception(message)
{
}

FileTransferException::FileTransferException(const Exception* baseException)
  : Exception(baseException->getMessage())
{
}

FileTransferException::~FileTransferException()
{
}
