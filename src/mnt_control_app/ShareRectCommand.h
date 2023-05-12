#pragma once
#include "utils/Command.h"
#include "ControlProxy.h"

class ShareRectCommand : public Command
{
public:
  ShareRectCommand(ControlProxy* serverControl,
    const Rect* shareRect);
  virtual ~ShareRectCommand();

  virtual void execute();
private:
  ControlProxy* m_proxy;
  const Rect m_shareRect;
};

