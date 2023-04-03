#pragma once
#include "Encoder.h"

class HextileEncoder: public Encoder
{
public:
  HextileEncoder(PixelConverter* conv, DataOutputStream* output);
  virtual ~HextileEncoder();

  virtual int getCode() const;

  virtual void sendRectangle(const Rect* rect, const FrameBuffer* serverFb, const EncodeOptions* options) throw(IOException);

private:
  template <class PIXEL_T> void hextileFunction(const Rect& r, const FrameBuffer* frameBuffer) throw(IOException);
};

