#include "WindowsScreenGrabber.h"
#include "Utils/Macros.h"

WindowsScreenGrabber::WindowsScreenGrabber(void)
  : m_destDC(NULL), m_screenDC(NULL), m_hbmDIB(NULL), m_hbmOld(NULL)
{
  m_serverConfig = Configurator::getInstance()->getServerConfig();
  setWorkRectDefault();
  resume();
  m_hasStartedSignal.waitForEvent();
}

WindowsScreenGrabber::~WindowsScreenGrabber(void)
{
  closeDIBSection();
}

bool WindowsScreenGrabber::grab(const Rect* rect)
{
  if (rect != NULL) {
    return grabByDIBSection(rect);
  }

  Rect grabRect;
  Dimension workDim = m_workFrameBuffer.getDimension();
  // Set relative co-ordinates
  grabRect.left = 0;
  grabRect.top = 0;
  grabRect.setWidth(workDim.width);
  grabRect.setHeight(workDim.height);

  return grabByDIBSection(&grabRect);
}

inline bool WindowsScreenGrabber::getPropertiesChanged()
{
  // Check for changing
  if (getScreenSizeChanged() || getPixelFormatChanged()) {
    return true;
  }

  return false;
}

inline bool WindowsScreenGrabber::getPixelFormatChanged()
{
  m_screen.update();

  PixelFormat currentPF = m_screen.getPixelFormat();
  PixelFormat frameBufferPF = m_workFrameBuffer.getPixelFormat();

  return !frameBufferPF.isEqualTo(&currentPF);
}

inline bool WindowsScreenGrabber::getScreenSizeChanged()
{
  m_screen.update();

  Rect screenRect = m_screen.getDesktopRect();
  int width = screenRect.getWidth();
  int height = screenRect.getHeight();

  if (width != m_fullScreenRect.getWidth() ||
    height != m_fullScreenRect.getHeight()) {
    return true;
  }

  int left = screenRect.left;
  int top = screenRect.top;

  if (left != m_fullScreenRect.left ||
    top != m_fullScreenRect.top) {
    // In this case apply new properties automatically
    // and don't inform anybody.
    applyNewProperties();
  }

  return false;
}

bool WindowsScreenGrabber::applyNewFullScreenRect()
{
  m_screen.update();
  m_fullScreenRect = m_screen.getDesktopRect();
  setWorkRect(&m_fullScreenRect);

  return true;
}

bool WindowsScreenGrabber::applyNewPixelFormat()
{
  m_screen.update();
  m_workFrameBuffer.setEmptyPixelFmt(&unmove(m_screen.getPixelFormat()));

  return true;
}

bool WindowsScreenGrabber::applyNewProperties()
{
  if (!applyNewPixelFormat() || !applyNewFullScreenRect() || !openDIBSection())
  {
    return false;
  }

  return true;
}

void WindowsScreenGrabber::execute()
{
  applyNewProperties();
  m_hasStartedSignal.notify();
  m_threadStopper.waitForEvent();
}

void WindowsScreenGrabber::onTerminate()
{
  m_threadStopper.notify();
}


bool WindowsScreenGrabber::openDIBSection()
{
  closeDIBSection();

  m_workFrameBuffer.setBuffer(0);

  m_screenDC = GetDC(0);
  if (m_screenDC == NULL) {
    return false;
  }

  if (getPropertiesChanged()) {
    return false;
  }
  
  Screen::BMI bmi;
  try {
    m_screen.getBMI(&bmi, m_screenDC);
  } catch (...) {
    return false;
  }

  PixelFormat pixelFormat = m_workFrameBuffer.getPixelFormat();
  Dimension workDim = m_workFrameBuffer.getDimension();

  bmi.bmiHeader.biBitCount = pixelFormat.bitsPerPixel;
  bmi.bmiHeader.biHeight = workDim.width;
  bmi.bmiHeader.biHeight = -workDim.height;
  bmi.bmiHeader.biCompression = BI_BITFIELDS;
  bmi.red = pixelFormat.redMax << pixelFormat.redShift;
  bmi.green = pixelFormat.greenMax << pixelFormat.greenShift;
  bmi.blue = pixelFormat.blueMax << pixelFormat.blueShift;

  m_destDC = CreateCompatibleDC(NULL);
  if (m_destDC == NULL) {
    DeleteDC(m_screenDC);
    return false;
  }

  void* buffer;
  m_hbmDIB = CreateDIBSection(m_destDC, (BITMAPINFO*)&bmi, DIB_RGB_COLORS, &buffer, NULL, NULL);
  if (m_hbmDIB == 0) {
    DeleteDC(m_destDC);
    DeleteDC(m_screenDC);
    return false;
  }
  m_workFrameBuffer.setBuffer(buffer);
  m_dibSectionDim = workDim;

  m_hbmOld = (HBITMAP)SelectObject(m_destDC, m_hbmDIB);

  return true;
}

bool WindowsScreenGrabber::closeDIBSection()
{
  // Free resources
  SelectObject(m_destDC, m_hbmOld);

  DeleteObject(m_hbmDIB);
  m_hbmDIB = NULL;

  DeleteDC(m_destDC);
  m_destDC = NULL;

  DeleteDC(m_screenDC);
  m_screenDC = NULL;

  m_workFrameBuffer.setBuffer(NULL);;
  return true;
}

bool WindowsScreenGrabber::grabByDIBSection(const Rect* rect)
{
  Dimension workDim = m_workFrameBuffer.getDimension();
  if (workDim.width != m_dibSectionDim.width ||
    workDim.height != m_dibSectionDim.height) {
    if (!openDIBSection()) {
      return false;
    }
  }

  DWORD bitBltFlag;
  if (m_serverConfig->getGrabTransparentWindowsFlag()) {
    bitBltFlag = SRCCOPY | CAPTUREBLT;
  } else {
    bitBltFlag = SRCCOPY;
  }

  if (BitBlt(m_destDC, rect->left, rect->top, rect->getWidth(), rect->getHeight(),
    m_screenDC, rect->left + m_offsetFrameBuffer.x,
    rect->top + m_offsetFrameBuffer.y, bitBltFlag) == 0) {
    return false;
  }

  return !getPropertiesChanged();
}
