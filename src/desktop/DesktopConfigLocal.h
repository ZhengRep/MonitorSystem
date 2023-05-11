#pragma once
#include "InputBlocker.h"
#include "log_writer/LogWriter.h"

class DesktopConfigLocal
{
public:
  DesktopConfigLocal(LogWriter* log);
  virtual ~DesktopConfigLocal();

  virtual void updateByNewSettings();
  virtual bool isRemoteInputAllowed();
  virtual DateTime getLastInputTime() const;
  virtual void correctLastTime(DateTime newTime);

private:
  InputBlocker* m_inputBlocker;
};

