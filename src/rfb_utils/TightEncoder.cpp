#include "TightEncoder.h"
#include "io_lib/ByteArrayOutputStream.h"

TightEncoder::TightEncoder(PixelConverter* conv, DataOutputStream* output)
  :Encoder(conv, output)
{
}

TightEncoder::~TightEncoder()
{
}

int TightEncoder::getCode() const
{
  return 0;
}

void TightEncoder::splitRectangle(const Rect* rect, std::vector<Rect>* rectList, const FrameBuffer* serverFb, const EncodeOptions* options)
{
}

void TightEncoder::sendRectangle(const Rect* rect, const FrameBuffer* serverFb, const EncodeOptions* options) throw(IOException)
{
}

void TightEncoder::sendSolidRect(const Rect* r, const FrameBuffer* fb) throw(IOException)
{
}

void TightEncoder::sendJpegRect(const Rect* rect, const FrameBuffer* serverFb, const EncodeOptions* options) throw(IOException)
{
}

bool TightEncoder::shouldPackPixels(const PixelFormat* pf) const
{
  return false;
}

void TightEncoder::packPixels(UINT8* buf, int count, const PixelFormat* pf)
{
}

void TightEncoder::sendCompressed(const char* data, size_t dataLen, int streamId, int zlibLevel) throw(IOException)
{
}

void TightEncoder::sendCompactLength(size_t dataLen) throw(IOException)
{
}
