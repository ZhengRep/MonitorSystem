#pragma once
#include "Utils/Exception.h"
#include "InputStream.h"
#include "OutputStream.h"

class Channel: public InputStream, OutputStream
{
public:
  virtual ~Channel();
  virtual void close() throw(Exception) = 0;
};

