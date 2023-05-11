#pragma once
#include "io_lib/IOException.h"

class FileNotFoundException : public IOException
{
public:
  FileNotFoundException();
  FileNotFoundException(const TCHAR* message);
  virtual ~FileNotFoundException();
};

