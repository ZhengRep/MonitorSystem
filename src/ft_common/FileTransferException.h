#pragma once
#include "utils/Exception.h"

class FileTransferException: public Exception
{
public:
  FileTransferException();
  FileTransferException(const TCHAR* message);
  FileTransferException(const Exception* baseException);
  virtual ~FileTransferException();
};

