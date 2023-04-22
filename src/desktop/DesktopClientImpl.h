#pragma once
#include "Utils/CommonHeader.h"
#include "DesktopServerWatcher.h"
#include "desktop_ipc/DesktopConfigClient.h"
#include "desktop_ipc/ReconnectingChannel.h"
#include "desktop_ipc/BlockingGate.h"
#include "desktop_ipc/GateKicker.h"
#include "desktop_ipc/DesktopSrvDispatcher.h"
#include "DesktopBaseImpl.h"
#include "desktop_ipc/ReconnectionListener.h"
#include "UpdateListener.h"
#include "Utils/AnEventListener.h"

class DesktopClientImpl: public AnEventListener, public ReconnectionListener,
  public Thread, public 
{
}; 

