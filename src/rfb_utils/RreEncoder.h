#pragma once
#include "Encoder.h"
#include "region/Region.h"
#include <vector>
using namespace std;

class RreEncoder : public Encoder
{
public:
  RreEncoder(PixelConverter* conv, DataOutputStream* output);
  virtual ~RreEncoder();

  virtual int getCode() const;

  virtual void splitRectangle(const Rect* rect, std::vector<Rect>* rectList, const FrameBuffer* serverFb, const EncodeOptions* options);

  virtual void sendRectangle(const Rect* rect, const FrameBuffer* serverFb, const EncodeOptions* options) throw(IOException);

private:
  template <class PIXEL_T> void rreEncode(const Rect* r, const FrameBuffer* frameBuffer) throw(IOException);

  // Coordinates of subrectangles.
  std::vector<Rect> m_rects;

  // All rectangles are devided (in splitRectangle() function)
  // into new rectangles with maximum size == 64.
  // Rect size == 64 for a better performance and less memory consumption.
  static const int RECT_SIZE = 64;
};

