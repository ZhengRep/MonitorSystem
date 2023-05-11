#pragma once
#include "Desktop.h"
#include "UpdateHandler.h"
#include "server_config_lib/ConfigReloadListener.h"
#include "UserInput.h"
#include "AbnormDeskTermListener.h"
#include "UpdateSendingListener.h"
#include "ClipboardListener.h"

class DesktopBaseImpl: public Desktop,public UpdateListener,
  public ClipboardListener, public ConfigReloadListener
{
public:
  DesktopBaseImpl(ClipboardListener* extClipListener, UpdateSendingListener* extUpdSendingListener,
    AbnormDeskTermListener* extDeskTermListener, LogWriter* log);
  virtual ~DesktopBaseImpl();

  virtual void getCurrentUserInfo(StringStorage* desktopName, StringStorage* userName);
  virtual void getFrameBufferProperties(Dimension* dim, PixelFormat* pf);
  virtual void getPrimaryDesktopCoords(Rect* rect);
  virtual void getDisplayNumberCoords(Rect* rect,
    unsigned char dispNumber);
  virtual void getNormalizedRect(Rect* rect);
  virtual void getWindowCoords(HWND hwnd, Rect* rect);
  virtual HWND getWindowHandleByName(const StringStorage* windowName);
  virtual void getApplicationRegion(unsigned int procId, Region* region);
  virtual bool isApplicationInFocus(unsigned int procId);

  virtual void setKeyboardEvent(UINT32 keySym, bool down);
  virtual void setMouseEvent(UINT16 x, UINT16 y, UINT8 buttonMask);
  virtual void setNewClipText(const StringStorage* newClipboard);

protected:
  virtual void onUpdate();
  virtual void onUpdateRequest(const Rect* rectReqeusted, bool incremental);
  virtual void onClipboardUpdate(const StringStorage* newClipboard);
  virtual void onConfigReload(ServerConfig* serverConfig);
  virtual void applyNewConfiguration() = 0;
  bool isRemoteInputAllowed();
  virtual bool isRemoteInputTempBlocked() = 0;

  virtual bool updateExternalFrameBuffer(FrameBuffer* fb, const Region* region, const Rect* viewPort);
  void sendUpdate();

  Region m_fullReqRegion;
  LocalMutex m_reqRegMutex;
  UpdateHandler* m_updateHandler;

  WindowsEvent m_newUpdateEvent;
  UserInput* m_userInput;

  StringStorage m_receivedClip;
  LocalMutex m_storedClipCritSec;

  UpdateSendingListener* m_extUpdSendingListener;
  AbnormDeskTermListener* m_extDeskTermListener;
  ClipboardListener* m_extClipListener;

  LogWriter* m_log;
};