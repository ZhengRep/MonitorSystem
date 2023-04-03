#include "Encoder.h"
#include <crtdbg.h>

Encoder::Encoder(PixelConverter* conv, DataOutputStream* output)
  :m_pixelConverter(conv),m_output(output)
{
}

Encoder::~Encoder(void)
{
}

int Encoder::getCode() const
{
  return EncodingDefs::RAW;
}

void Encoder::splitRectangle(const Rect* rect, std::vector<Rect>* rectList, const FrameBuffer* serverFb, const EncodeOptions* options)
{
  const FrameBuffer* fb = m_pixelConverter->convert(rect, serverFb);
  int pixelSize = fb->getBytesPerPixel();
  _ASSERT(pixelSize == fb->getBytesPerPixel());

  UINT8* buffer = (UINT8*)fb->getBuffer();
  int lineWidth = rect->getWidth();
  int fbWidth = fb->getDimension().width;
  int lineSizeInBytes = lineWidth * pixelSize;
  int stride = fbWidth * pixelSize;
  UINT8* lineP = &buffer[(rect->top * fbWidth + rect->left) * pixelSize];

  for (int i = rect->top; i < rect->bottom; i++, lineP += stride) {
    m_output->writeFully((char*)lineP, lineSizeInBytes);
  }
}


