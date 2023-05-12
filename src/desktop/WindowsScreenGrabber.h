#pragma once
#include "utils/CommonHeader.h"

#ifndef CAPTUREBLT
#define CAPTUREBLT (DWORD)0x40000000 /*Include layered windows*/
#endif

#include "ScreenGrabber.h"
#include "region/Rect.h"
#include "server_config_lib/Configurator.h"
#include "win_system/WindowsEvent.h"
#include "thread/GuiThread.h"
#include "win_system/Screen.h"

class WindowsScreenGrabber: public ScreenGrabber, public GuiThread
{
public:
  WindowsScreenGrabber(void);
  virtual ~WindowsScreenGrabber(void);

  virtual bool grab(const Rect* rect = 0);

  inline virtual bool getPropertiesChanged();
  inline virtual bool getPixelFormatChanged();
  inline virtual bool getScreenSizeChanged();

  virtual bool applyNewFullScreenRect();
  virtual bool applyNewPixelFormat();
  virtual bool applyNewProperties();

protected:
  virtual void execute();
  virtual void onTerminate();

  virtual bool openDIBSection();
  virtual bool closeDIBSection();
  virtual bool grabByDIBSection(const Rect* rect);

  HDC m_destDC, m_screenDC;
  HBITMAP m_hbmOld, m_hbmDIB;

  WindowsEvent m_hasStartedSignal;
  WindowsEvent m_threadStopper;

private:
  Dimension m_dibSectionDim;
  ServerConfig* m_serverConfig;
  Screen m_screen;
};

