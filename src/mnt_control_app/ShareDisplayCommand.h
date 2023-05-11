#pragma once
#include "Utils/Command.h"
#include "ControlProxy.h"

class ShareDisplayCommand : public Command
{
public:
  ShareDisplayCommand(ControlProxy* serverControl,
    unsigned char displayNumber);
  virtual ~ShareDisplayCommand();

  virtual void execute();
private:
  ControlProxy* m_proxy;
  unsigned char m_displayNumber;
};

