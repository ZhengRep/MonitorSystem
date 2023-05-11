#pragma once
#include "Utils/CommonHeader.h"
#include "desktop/UserInput.h"
#include "desktop/ClipboardListener.h"
#include "Utils/inttypes.h"
#include "DesktopServerProto.h"
#include "DesktopSrvDispatcher.h"

class UserInputClient: public UserInput, public DesktopServerProto, public ClientListener
{
public:
  UserInputClient(BlockingGate* forwGate, DesktopSrvDispatcher* dispatcher, ClipboardListener* clipboardListener);
  virtual ~UserInputClient();

  virtual void sendInit(BlockingGate* gate);
  virtual void setNewClipboard(const StringStorage* newClipboard);
  virtual void setMouseEvent(const Point* newPos, UINT8 keyFlag);
  virtual void setKeyboardEvent(UINT32 keySym, bool down);
  virtual void getCurrentUserInfo(StringStorage* desktopName,
    StringStorage* userName);
  virtual void getPrimaryDisplayCoords(Rect* rect);
  virtual void getDisplayNumberCoords(Rect* rect,
    unsigned char dispNumber);
  virtual void getNormalizedRect(Rect* rect);
  virtual void getWindowCoords(HWND hwnd, Rect* rect);
  virtual HWND getWindowHandleByName(const StringStorage* windowName);
  virtual void getApplicationRegion(unsigned int procId, Region* region);
  virtual bool isApplicationInFocus(unsigned int procId);

  // To catch a new clipboard
  virtual void onRequest(UINT8 reqCode, BlockingGate* backGate);

protected:
  UINT8 m_sendMouseFlags;
  ClipboardListener* m_clipboardListener;
};

