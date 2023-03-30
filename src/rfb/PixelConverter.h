#pragma once
#include "FrameBuffer.h"
#include "region/Point.h"

class PixelConverter
{
public:
  PixelConverter(void);
  virtual ~PixelConverter(void);

  virtual void convert(const Rect* rect, FrameBuffer* dstFb, const FrameBuffer* srcFb) const;
  virtual const FrameBuffer* convert(const Rect* rect, const FrameBuffer* srcFb);
  virtual size_t getSrcBitsPerPixel() const;
  virtual size_t getDstBitsPerPixel() const;
  virtual void setPixelFormats(const PixelFormat* dstPf, const PixelFormat* srcPf);

protected:
  void reset();
  void fillHexBitsTable(const PixelFormat* dstPf, const PixelFormat* srcPf);
  void fill32BitsTable(const PixelFormat* dstPf, const PixelFormat* srcPf);
  UINT32 rotateUint32(UINT32 value) const;

  enum ConvertMode {
    NO_CONVERT,
    CONVERT_FROM_16,
    CONVERT_FROM_32
  };

  ConvertMode m_convertMode;
  std::vector<UINT32> m_hexBitsTable;
  std::vector<UINT32> m_redTable;
  std::vector<UINT32> m_grnTable;
  std::vector<UINT32> m_bluTable;

  PixelFormat m_srcFormat;
  PixelFormat m_dstFormat;

  FrameBuffer* m_dstFrameBuffer;
};

