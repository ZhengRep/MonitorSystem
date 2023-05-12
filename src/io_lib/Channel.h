#pragma once
#include "utils/Exception.h"
#include "InputStream.h"
#include "OutputStream.h"

class Channel: public InputStream, public OutputStream
{
public:
  virtual ~Channel();
  virtual void close() throw(Exception) = 0;
};

