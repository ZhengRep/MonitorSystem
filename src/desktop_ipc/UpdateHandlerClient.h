#pragma once
#include "desktop/UpdateHandler.h"
#include "DesktopServerProto.h"
#include "DesktopSrvDispatcher.h"
#include "log_writer/LogWriter.h"

class UpdateHandlerClient: public UpdateHandler, public DesktopServerProto, 
  public ClientListener
{
public:
  UpdateHandlerClient(BlockingGate* forwGate, DesktopSrvDispatcher* dispatcher,
    UpdateListener* externalUpdateListener, LogWriter* log);
  virtual ~UpdateHandlerClient();

  virtual void extract(UpdateContainer* updateContainer);
  virtual void setFullUpdateRequested(const Region* region);
  virtual void setExcludedRegion(const Region* excludedRegion);
  virtual bool checkForUpdates(Region* region);

protected:
  virtual void getScreenProperties(PixelFormat* pf, Dimension* dim);
  virtual void sendInit(BlockingGate* gate);

  // To catch update event
  virtual void onRequest(UINT8 reqCode, BlockingGate* backGate);

  UpdateListener* m_externalUpdateListener;

  LogWriter* m_log;
};

