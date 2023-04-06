#include "JpegEncoder.h"

JpegEncoder::JpegEncoder(TightEncoder* tightEncoder)
  :Encoder(tightEncoder->m_pixelConverter, tightEncoder->m_output)
{
}

JpegEncoder::~JpegEncoder()
{
}

int JpegEncoder::getCoder() const
{
  return m_tightEncoder->getCode();
}

void JpegEncoder::splitRectangle(const Rect* rect, std::vector<Rect>* rectList, const FrameBuffer* serverFb, const EncodeOptions* options)
{
  int maxWidth = 2048;
  for (int x0 = rect->left; x0 < rect->right; x0 += maxWidth) {
    int x1 = (x0 + maxWidth <= rect->right) ? x0 + maxWidth : rect->right;
    rectList->push_back(Rect(x0, rect->top, x1, rect->bottom));
  }
}

void JpegEncoder::sendRectangle(const Rect* rect, const FrameBuffer* serverFb, const EncodeOptions* options)
{
  size_t bppServer = m_pixelConverter->getSrcBitsPerPixel();
  size_t bppClient = m_pixelConverter->getDstBitsPerPixel();

  bool goodColorResolution = (bppServer >= 16 && bppClient >= 16);
  if (options->jpegEnabled() && goodColorResolution) {
    m_tightEncoder->sendJpegRect(rect, serverFb, options);
  } else {
    m_tightEncoder->sendRectangle(rect, serverFb, options);
  }
}
