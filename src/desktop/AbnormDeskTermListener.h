#pragma once

class AbnormDeskTermListener {
public:
  virtual ~AbnormDeskTermListener() {};
  virtual void onAbnormalDesktopTerminate() = 0;
};