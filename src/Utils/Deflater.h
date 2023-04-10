#pragma once
#include "ZLibBase.h"
#include "ZLibException.h"

class Deflater: public ZLibBase
{
public:
  Deflater();
  ~Deflater();

  void deflate() throw(ZLibException);
protected:
  z_stream m_zlibStream;
};

