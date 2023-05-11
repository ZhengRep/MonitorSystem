#pragma once
#include "rfb/FrameBuffer.h"
#include <dxgi1_2.h>

class WinCursorShapeUtils
{
public:
  WinCursorShapeUtils();
  virtual ~WinCursorShapeUtils();


  // Builds rfb mask AND by alpha channel of given pixels.
  // If isInversedAlpha is false the pixel supposes by transparent if alpha value less than 128.
  // If isInversedAlpha is true the pixel supposes by transparent if alpha value greater than 128.
  static void fixAlphaChannel(const FrameBuffer* pixels, char* maskAND, bool isInversedAlpha, int maskWidthInBytes);

  // This function combines windows cursor mask and image and convert
  // theirs to rfb format. This function uses for monochrome cursor image.
  static void winMonoShapeToRfb(const FrameBuffer* pixels, char* maskAND, char* maskXOR, int maskWidthInBytes);

  //   This function combines windows the cursor mask and image and convert
  // theirs to rfb format. This function uses for 16 or 24 bit color cursor
  // image.
  //   Also, this function determines whether image contains the alhpa channel
  // and returns true in this case.
  template< typename T >
  static bool winColorShapeToRfb(const FrameBuffer* pixels, char* maskAND, int maskWidthInBytes);

  // Matrox videocard returns 256 byte width buffer for 32 pixel cursor,
  // need trim it for correct handling
  static void trimBuffer(std::vector<char>* buffer, DXGI_OUTDUPL_POINTER_SHAPE_INFO* shapeInfo);

private:
  // Inverts bit array with the "not" operator.
  static void inverse(char* bits, int count);

  // Returns true if a bit on the index place is true. High-order bit has zero index.
  inline static bool testBit(char byte, int index) {
    char dummy = 128 >> index;
    return (dummy & byte) != 0;
  };

  inline static char setBit(char byte, int index) {
    char dummy = 128 >> index;
    return dummy | byte;
  };

  inline static char clearBit(char byte, int index) {
    char dummy = 128 >> index;
    return ~dummy & byte;
  };

  static UINT32 getAlphaMask(const PixelFormat* pf);
  static UINT getCursorHeight(DXGI_OUTDUPL_POINTER_SHAPE_INFO& shapeInfo);
  static bool isPixelTransparent(char* const buffer, UINT type, UINT height, UINT pitch, UINT x, UINT y);
  static bool isColorPixelTransparent(UINT32 pixel, UINT type);
  static bool isMonochromePixelTransparent(char andByte, char xorByte, UINT x);
  static void trimTransparent(std::vector<char>* buffer, DXGI_OUTDUPL_POINTER_SHAPE_INFO* shapeInfo);
};

template< typename T >
static bool WinCursorShapeUtils::winColorShapeToRfb(const FrameBuffer* pixels,
  char* maskAND, int maskWidthInBytes)
{
  char* pixelsBuffer = (char*)pixels->getBuffer();
  PixelFormat pf = pixels->getPixelFormat();
  T* pixel;

  int fbWidth = pixels->getDimension().width;
  int fbHeight = pixels->getDimension().height;

  bool hasAlphaChannel = false;
  UINT32 alphaMask = getAlphaMask(&pf);

  for (int iRow = 0; iRow < fbHeight; iRow++) {
    for (int iCol = 0; iCol < fbWidth; iCol++) {
      pixel = (T*)pixelsBuffer + iRow * fbWidth + iCol;

      size_t iMaskAnd = iRow * maskWidthInBytes + iCol / 8;
      bool maskANDBit = testBit(maskAND[iMaskAnd], iCol % 8);

      if (!maskANDBit) { // *pixel = *pixel
        // Set current mask bit to true
        maskAND[iMaskAnd] = setBit(maskAND[iMaskAnd], iCol % 8);
      } else if ((*pixel >> pf.redShift & pf.redMax) == 0 &&
        (*pixel >> pf.greenShift & pf.greenMax) == 0 &&
        (*pixel >> pf.blueShift & pf.blueMax) == 0) { // Transparent dot
        maskAND[iMaskAnd] = clearBit(maskAND[iMaskAnd], iCol % 8);
      } else { // Inverted (as black dot)
        // Set current mask bit to true
        maskAND[iMaskAnd] = setBit(maskAND[iMaskAnd], iCol % 8);
        // Set pixel to black with the alpa channel preserving
        *pixel &= ~((T)pf.redMax << pf.redShift);
        *pixel &= ~((T)pf.greenMax << pf.greenShift);
        *pixel &= ~((T)pf.blueMax << pf.blueShift);
      }
      // Test for the alpha channel presence
      hasAlphaChannel = hasAlphaChannel || (*pixel & alphaMask) != 0;
    }
  }
  return hasAlphaChannel;
}

