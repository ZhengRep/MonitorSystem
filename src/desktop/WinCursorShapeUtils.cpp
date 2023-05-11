#include "WinCursorShapeUtils.h"

WinCursorShapeUtils::WinCursorShapeUtils()
{
}

WinCursorShapeUtils::~WinCursorShapeUtils()
{
}

void WinCursorShapeUtils::winMonoShapeToRfb(const FrameBuffer* pixels,
  char* maskAND, char* maskXOR, int maskWidthInBytes)
{
  char* pixelsBuffer = (char*)pixels->getBuffer();
  char* pixel;
  int pixelSize = pixels->getBytesPerPixel();
  int pixelCount = pixels->getBufferSize() / pixelSize;

  int fbWidth = pixels->getDimension().width;
  int fbHeight = pixels->getDimension().height;

  for (int iRow = 0; iRow < fbHeight; iRow++) {
    for (int iCol = 0; iCol < fbWidth; iCol++) {
      pixel = pixelsBuffer + (iRow * fbWidth + iCol) * pixelSize;

      char byteAnd = maskAND[iRow * maskWidthInBytes + iCol / 8];
      char byteXor = maskXOR[iRow * maskWidthInBytes + iCol / 8];

      bool maskANDBit = testBit(byteAnd, iCol % 8);
      bool maskXORBit = testBit(byteXor, iCol % 8);

      if (!maskANDBit && !maskXORBit) { // Black dot
        memset(pixel, 0, pixelSize);
      } else if (!maskANDBit && maskXORBit) { // White dot
        memset(pixel, 0xff, pixelSize);
      } else if (maskANDBit && maskXORBit) { // Inverted (as black dot)
        memset(pixel, 0, pixelSize);
      }
    }
  }

  inverse(maskAND, maskWidthInBytes * fbHeight);
  for (int i = 0; i < maskWidthInBytes * fbHeight; i++) {
    *(maskAND + i) |= *(maskXOR + i);
  }
}

void WinCursorShapeUtils::fixAlphaChannel(const FrameBuffer* pixels,
  char* maskAND, bool maskedColor, int maskWidthInBytes)
{
  PixelFormat pf = pixels->getPixelFormat();
  if (pf.bitsPerPixel != 32) {
    return;
  }
  UINT32 alphaMask = getAlphaMask(&pf);
  UINT16 alphaShift = 0;

  for (; alphaShift < 32 && ((alphaMask >> alphaShift) % 2) == 0; alphaShift++) {}

  if (alphaShift > 24) { // we don't know how to handle it
    return;
  }

  UINT32* pixelBuffer = (UINT32*)pixels->getBuffer();
  int pixelSize = pixels->getBytesPerPixel();

  int fbWidth = pixels->getDimension().width;
  int fbHeight = pixels->getDimension().height;

  for (int iRow = 0; iRow < fbHeight; iRow++) {
    for (int iCol = 0; iCol < fbWidth; iCol++) {
      UINT32* pixel = &pixelBuffer[iRow * fbWidth + iCol];
      UINT32 colorValue = *pixel & ~alphaMask;
      UINT32 alpha = (*pixel & alphaMask) >> alphaShift;
      bool transparent = (alpha < 128);
      if (maskedColor) {
        transparent = !transparent;
      }
      if (transparent) {
        char* byteAnd = &maskAND[iRow * maskWidthInBytes + iCol / 8];
        *byteAnd = clearBit(*byteAnd, iCol % 8);
      }
    }
  }
}


UINT32 WinCursorShapeUtils::getAlphaMask(const PixelFormat* pf)
{
  if (pf->bitsPerPixel == 32) {
    UINT32 alphaMax = pf->redMax << pf->redShift |
      pf->greenMax << pf->greenShift |
      pf->blueMax << pf->blueShift;
    return ~alphaMax;
  } else {
    return 0;
  }
}

void WinCursorShapeUtils::inverse(char* bits, int count)
{
  for (int i = 0; i < count; i++, bits++) {
    *bits = ~*bits;
  }
}

void WinCursorShapeUtils::trimBuffer(std::vector<char>* buffer, DXGI_OUTDUPL_POINTER_SHAPE_INFO* shapeInfo)
{
  UINT newPitch;
  UINT oldPitch = shapeInfo->Pitch;
  trimTransparent(buffer, shapeInfo);
  if (shapeInfo->Type == DXGI_OUTDUPL_POINTER_SHAPE_TYPE_MONOCHROME) {
    newPitch = ((shapeInfo->Width + 15) / 16) * 2;
  } else {
    newPitch = shapeInfo->Width * 4;
  }

  if (newPitch >= oldPitch) {
    return;
  }

  for (int i = 1; i < shapeInfo->Height; i++) {
    char* dst = &buffer->at(i * newPitch);
    char* src = &buffer->at(i * oldPitch);
    memcpy(dst, src, newPitch);
  }
  shapeInfo->Pitch = newPitch;
}

void WinCursorShapeUtils::trimTransparent(std::vector<char>* buffer, DXGI_OUTDUPL_POINTER_SHAPE_INFO* shapeInfo)
{
  UINT pitch = shapeInfo->Pitch;
  UINT height = getCursorHeight(*shapeInfo);
  UINT width = shapeInfo->Width;
  UINT hotspotX = (UINT)(shapeInfo->HotSpot.x);
  UINT type = shapeInfo->Type;

  // width 
  const UINT minimumWidth = 16;
  UINT trimmedWidth = minimumWidth;

  for (UINT y = 0; y < height; ++y) {
    for (UINT x = width - 1; x > trimmedWidth; --x) {
      if (!isPixelTransparent(&buffer->front(), type, height, pitch, x, y)) {
        trimmedWidth = x + 1;
      }
    }
  }

  // Force to be on a 2-byte word boundary
  trimmedWidth = ((trimmedWidth + 0xF) & ~0xF);

  if (trimmedWidth < shapeInfo->Width) {
    shapeInfo->Width = trimmedWidth;
  }

  // height
  UINT trimmedHeight = minimumWidth;

  for (UINT x = 0; x < width; ++x) {
    for (UINT y = height - 1; y > trimmedHeight; --y) {
      if (!isPixelTransparent(&buffer->front(), type, height, pitch, x, y)) {
        trimmedHeight = y + 1;
      }
    }
  }
  if (trimmedHeight != height) {
    if (type == DXGI_OUTDUPL_POINTER_SHAPE_TYPE_MONOCHROME) {
      shapeInfo->Height = (2 * trimmedHeight);

      // Shift the XOR mask
      char* dst = &buffer->at(trimmedHeight * pitch);
      char* src = &buffer->at(height * pitch);
      memcpy(dst, src, trimmedHeight * pitch);
    } else {
      shapeInfo->Height = trimmedHeight;
    }
  }
}

bool WinCursorShapeUtils::isMonochromePixelTransparent(char andByte, char xorByte, UINT x)
{
  bool pixelSet = WinCursorShapeUtils::testBit(andByte, x % 8);
  bool xorSet = WinCursorShapeUtils::testBit(xorByte, x % 8);;
  bool transparent = (pixelSet && !xorSet);

  return transparent;
}

bool WinCursorShapeUtils::isColorPixelTransparent(UINT32 pixel, UINT type)
{
  bool transparent;
  // color data is 32 bpp ARGB DIB
  const UINT32 alphaMask = 0xFF000000;
  const UINT32 transparencyThreshold = 0x800000;
  const UINT32 colorMask = 0x00FFFFFF;

  if (type == DXGI_OUTDUPL_POINTER_SHAPE_TYPE_COLOR) {
    transparent = ((pixel & alphaMask) < transparencyThreshold);
  } else {
    UINT32 color = (pixel & colorMask);
    bool xorSet = ((pixel & alphaMask) != 0);	// XOR value can only be 0x00 (overwrite) or 0xFF (transparent)
    transparent = (xorSet && (color == 0));
  }

  return transparent;
}

bool WinCursorShapeUtils::isPixelTransparent(char* const buffer, UINT type, UINT height, UINT pitch, UINT x, UINT y)
{
  if (type == DXGI_OUTDUPL_POINTER_SHAPE_TYPE_MONOCHROME) {
    UINT andOffset = (y * pitch) + (x / 8);
    UINT xorOffset = andOffset + height * pitch;
    char andByte = buffer[andOffset];
    char xorByte = buffer[xorOffset];

    return isMonochromePixelTransparent(andByte, xorByte, x);
  }

  UINT offset = (y * pitch) + (x * 4);
  UINT32* pixel = (UINT32*)(&buffer[offset]);

  return isColorPixelTransparent(*pixel, type);
}

UINT WinCursorShapeUtils::getCursorHeight(DXGI_OUTDUPL_POINTER_SHAPE_INFO& shapeInfo)
{
  if (shapeInfo.Type == DXGI_OUTDUPL_POINTER_SHAPE_TYPE_MONOCHROME) {
    return shapeInfo.Height /= 2;
  }

  return shapeInfo.Height;
}