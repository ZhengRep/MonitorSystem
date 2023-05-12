#pragma once
#include "utils/CommonHeader.h"
#include "region/Point.h"
#include "desktop_ipc/BlockingGate.h"
#include "region/Rect.h"
#include "region/Region.h"

class UserInput
{
public:
  UserInput();
  virtual ~UserInput();

  //UserInputClient class
  virtual void sendInit(BlockingGate* gate){}
  //Client to server user inputs
  virtual void setNewClipboard(const StringStorage* newClipboard) = 0;
  virtual void setMouseEvent(const Point* newPos, UINT8 keyFlag) = 0;
  virtual void setKeyboardEvent(UINT32 keySym, bool down) = 0;
  virtual void getCurrentUserInfo(StringStorage* desktopName, StringStorage* userName) = 0;

  virtual void getPrimaryDisplayCoords(Rect* rect) = 0;
  virtual void getDisplayNumberCoords(Rect* rect, unsigned char dispNumber) = 0;
  virtual void getNormalizedRect(Rect* rect) = 0;

  virtual void getWindowCoords(HWND hwnd, Rect* rect) = 0;
  virtual HWND getWindowHandleByName(const StringStorage* windowName) = 0;

  virtual void getApplicationRegion(unsigned int procId, Region* region) = 0;
  virtual bool isApplicationInFocus(unsigned int procId) = 0;

};

