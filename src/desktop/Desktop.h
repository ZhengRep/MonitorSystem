#pragma once
#include "Utils/StringStorage.h"
#include "region/Dimension.h"
#include "region/Region.h"
#include "rfb/PixelFormat.h"
#include "rfb/FrameBuffer.h"
#include "fb_update_sender/UpdateRequestListener.h"

class Desktop : public UpdateRequestListener{
public:
  virtual ~Desktop() {};
  virtual void getCurrentUserInfo(StringStorage* desktopName, StringStorage* userName) = 0;
  virtual void getFrameBufferProperties(Dimension* dim, PixelFormat* pf) = 0;
  virtual void getPrimaryDesktopCoords(Rect* rect) = 0;
  virtual void getNormalizedRect(Rect* rect) = 0;
  virtual void getDisplayNumberCoords(Rect* rect, unsigned char displayNumber) = 0;
  virtual void getWindowCoords(HWND hwnd, Rect* rect) = 0;
  virtual HWND getWindowHandleByName(const StringStorage* windowName) = 0;

  virtual void getApplicationRegion(unsigned int procId, Region* region) = 0;
  virtual void isApplicationInFocus(unsigned int procId) = 0;

  virtual void setKeyboardEvent(UINT32 keySym, bool down) = 0;
  virtual void setMouseEvent(UINT16 x, UINT16 y, UINT8 buttonMask) = 0;
  virtual void setNewClipText(const StringStorage* newClipboard) = 0;

  virtual bool updateExternalFrameBuffer(FrameBuffer* fb, const Region* region, const Rect* viewPort) = 0;
};