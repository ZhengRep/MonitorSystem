#pragma once
#include "ScreenDriver.h"
#include "UpdateKeeper.h"
#include "UpdateListener.h"
#include "log_writer/LogWriter.h"

class ScreenDriverFactory
{
public:
  virtual ScreenDriver* createScreenDriver(UpdateKeeper* updateKeeper, UpdateListener* updateListener,
    FrameBuffer* fb, LocalMutex* fbLocalMutex, LogWriter* log) = 0;
};