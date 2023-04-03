#pragma once
#include "rfb/FrameBuffer.h"
#include "io_lib/IOException.h"
#include "network/RfbOutputGate.h"
#include "rfb/EncodingDefs.h"
#include "EncodeOptions.h"
#include "rfb/PixelConverter.h"


class Encoder
{
public:
  Encoder(PixelConverter* conv, DataOutputStream* output);
  virtual ~Encoder(void);
  virtual int getCode() const;

  virtual void splitRectangle(const Rect* rect, std::vector<Rect>* rectList, const FrameBuffer* serverFb, const EncodeOptions* options);

protected:
  PixelConverter* m_pixelConverter;
  DataOutputStream* m_output;

private:
  Encoder(const Encoder& other);
  Encoder& operator=(const Encoder& other);
};

