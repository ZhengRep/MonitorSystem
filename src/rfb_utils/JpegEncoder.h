#pragma once
#include "TightEncoder.h"

class JpegEncoder: public Encoder
{
public:
  JpegEncoder(TightEncoder* tightEncoder);
  virtual ~JpegEncoder();
  virtual int getCoder() const;
  virtual void splitRectangle(const Rect* rect, std::vector<Rect>* rectList, const FrameBuffer* serverFb, const EncodeOptions* options);
  virtual void sendRectangle(const Rect* rect, const FrameBuffer* serverFb, const EncodeOptions* options);

protected:
  TightEncoder* m_tightEncoder;
};

