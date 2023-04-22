#include "GuiThread.h"

GuiThread::GuiThread()
  :m_hDesk(0)
{
  m_hDesk = DesktopSelector::getInputDesktop();
}

GuiThread::~GuiThread()
{
  if (m_hDesk) {
    DesktopSelector::closeDesktop(m_hDesk);
  }
}

void GuiThread::initByDerived()
{
  DesktopSelector::setDesktopToCurrentThread(m_hDesk);
  // If unsuccessful, desktop will be closed in destructor
  if (DesktopSelector::closeDesktop(m_hDesk)) {
    m_hDesk = 0;
  }
}
