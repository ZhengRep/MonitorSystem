#include "HextileEncoder.h"
#include "HextileTile.h"
#include "utils/Macros.h"

HextileEncoder::HextileEncoder(PixelConverter* conv, DataOutputStream* output)
  :Encoder(conv, output)
{
}

HextileEncoder::~HextileEncoder()
{
}

int HextileEncoder::getCode() const
{
  return EncodingDefs::HEXTILE;
}

void HextileEncoder::sendRectangle(const Rect* rect, const FrameBuffer* serverFb, const EncodeOptions* options) throw(IOException)
{
  const FrameBuffer* fb = m_pixelConverter->convert(rect, serverFb);

  size_t bpp = fb->getBitsPerPixel();
  if (bpp == 8) {
    hextileFunction<UINT8>(*rect, fb);
  } else if (bpp == 16) {
    hextileFunction<UINT16>(*rect, fb);
  } else if (bpp == 32) {
    hextileFunction<UINT32>(*rect, fb);
  } else {
    _ASSERT(0);
  }
}

template<class PIXEL_T>
inline void HextileEncoder::hextileFunction(const Rect& r, const FrameBuffer* frameBuffer) throw(IOException)
{
  Rect t;
  PIXEL_T* buf;
  FrameBuffer fb;
  PIXEL_T oldBg = 0, oldFg = 0;
  bool oldBgValid = false;
  bool oldFgValid = false;
  UINT8 encoded[256 * sizeof(PIXEL_T)];

  HextileTile<PIXEL_T> tile;

  for (t.top = r.top; t.top < r.bottom; t.top += 16) {
    t.bottom = min(r.bottom, t.top + 16);
    for (t.left = r.left; t.left < r.right; t.left += 16) {
      t.right = min(r.right, t.left + 16);

      fb.setProperties(&t, &unmove(frameBuffer->getPixelFormat()));
      fb.copyFrom(frameBuffer, t.left, t.top);
      buf = (PIXEL_T*)fb.getBuffer();

      tile.newTile(buf, t.getWidth(), t.getHeight());
      int tileType = tile.getFlags();
      size_t encodedLen = tile.getSize();

      if ((tileType & hextileRaw) != 0 ||
        encodedLen >= t.getWidth() * t.getHeight() * sizeof(PIXEL_T)) {
        m_output->writeUInt8(hextileRaw);
        m_output->writeFully((char*)buf, t.getWidth() * t.getHeight() * sizeof(PIXEL_T));
        oldBgValid = oldFgValid = false;
        continue;
      }

      PIXEL_T bg = tile.getBackground();
      PIXEL_T fg = 0;

      if (!oldBgValid || oldBg != bg) {
        tileType |= hextileBgSpecified;
        oldBg = bg;
        oldBgValid = true;
      }

      if (tileType & hextileAnySubrects) {
        if (tileType & hextileSubrectsColoured) {
          oldFgValid = false;
        } else {
          fg = tile.getForeground();
          if (!oldFgValid || oldFg != fg) {
            tileType |= hextileFgSpecified;
            oldFg = fg;
            oldFgValid = true;
          }
        }
        tile.encode(encoded);
      }

      m_output->writeUInt8(tileType);
      if (tileType & hextileBgSpecified) {
        m_output->writeFully(&bg, sizeof(PIXEL_T));
      }
      if (tileType & hextileFgSpecified) {
        m_output->writeFully(&fg, sizeof(PIXEL_T));
      }
      if (tileType & hextileAnySubrects) {
        m_output->writeFully(encoded, encodedLen);
      }
    }
  }
}

