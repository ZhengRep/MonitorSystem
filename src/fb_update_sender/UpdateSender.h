#pragma once
#include "thread/AutoLock.h"
#include "thread/Thread.h"
#include "desktop/UpdateKeeper.h"
#include "UpdateRequestListener.h"
#include "ViewPort.h"
#include "network/RfbOutputGate.h"
#include "network/RfbInputGate.h"
#include "rfb_utils/Encoder.h"
#include "rfb_utils/HextileEncoder.h"
#include "rfb_utils/JpegEncoder.h"
#include "rfb_utils/EncoderStore.h"
#include "rfb_utils/RfbCodeRegistrator.h"
#include "utils/DateTime.h"
#include "CursorUpdates.h"
#include "SenderControlInformationInterface.h"
#include "log_writer/LogWriter.h"

class UpdateSender: public Thread, public RfbDispatcherListener
{
public:
  UpdateSender(RfbCodeRegistrator* codeRegtor, UpdateRequestListener* updReqListener, SenderControlInformationInterface* senderControlInformation,
    RfbOutputGate* output, int id, Desktop* desktop, LogWriter* log);
  virtual ~UpdateSender();
  void init(const Dimension* viewPortDimension, const PixelFormat* pf);
  void newUpdates(const UpdateContainer* updateContainer, const CursorShape* cursorShape);
  void blockCursorPosSending();
  Rect getViewPort();
  bool clientIsReady();

protected:
  virtual void onRequest(UINT32 reqCode, RfbInputGate* input);

  void readUpdateRequest(RfbInputGate* io);
  void readSetPixelFormat(RfbInputGate* io);
  void readSetEncodings(RfbInputGate* io);
  void readVideoFreeze(RfbInputGate* io);

  void addUpdateContainer(const UpdateContainer* updateContainer);
  virtual void execute();
  virtual void onTerminate();
  //void checkCursorPos(UpdateContainer* updCont, const Rect* viewPort);

  void setVideoFrozen(bool value);
  bool getVideoFrozen();

  void sendUpdate();

  bool extractReqRegions(Region* incrReqReg, Region* fullReqReg, bool* incrUpdIsReq, bool* fullUpdIsReq, DateTime* reqTimePoint);
  void extractUpdates(UpdateContainer* updCont);
  void cropUpdContForReqRegions(UpdateContainer* updCont, const Region* incrReqReg, const Region* fullReqReg);
  void inscribeCopiedRegionToReqRegion(UpdateContainer* updCont, const Region* requestRegion);

  void selectEncoder(EncodeOptions* encodeOptions);

  void updateFrameBuffer(UpdateContainer* updCont, bool shareOnlyApp, const Region* prevSharedRegion, const Region* shareAppRegion);
  bool updateViewPort(Rect* outNewViewPort, bool* shareApp, Region* prevShareAppRegion, Region* newShareAppRegion);

  void sendPalette(PixelFormat* pf);

  // Set new client pixel format. This function may be called from any thread.
  // New pixel format will take effect on sending next frame buffer update.
  void setClientPixelFormat(const PixelFormat* pf, bool clrMapEntries);

  void sendRectHeader(const Rect* rect, INT32 encodingType);
  void sendRectHeader(UINT16 x, UINT16 y, UINT16 w, UINT16 h, INT32 encodingType);
  void sendNewFBSize(Dimension* dim);
  void sendFbInClientDim(const EncodeOptions* encodeOptions, const FrameBuffer* fb, const Dimension* dim, const PixelFormat* pf);
  void sendCursorShapeUpdate(const PixelFormat* fmt, const CursorShape* cursorShape);
  void sendCursorPosUpdate();
  void sendCopyRect(const std::vector<Rect>* rects, const Point* source);

  void sendRectangles(Encoder* encoder, const std::vector<Rect>* rects, const FrameBuffer* frameBuffer, const EncodeOptions* encodeOptions);

  void paintBlack(FrameBuffer* frameBuffer, const Region* blackRegion);
  void splitRegion(Encoder* encoder, const Region* region, std::vector<Rect>* rects, const FrameBuffer* frameBuffer, const EncodeOptions* encodeOptions);
  Region takePartFromRegion(Region* reg, int area);
  int calcAreas(std::vector<Rect> rects);

  LogWriter* m_log;
  WindowsEvent m_newUpdatesEvent;
  UpdateRequestListener* m_updReqListener;
  Region m_requestedIncrReg;
  Region m_requestedFullReg;
  bool m_incrUpdIsReq;
  bool m_fullUpdIsReq;
  bool m_busy;

  //Property for performance measurements.
  DateTime m_requestTimePoint;
  LocalMutex m_reqRectLocMut;

  SenderControlInformationInterface* m_senderControlInformation;

  Rect m_viewPort;
  Dimension m_clientDim;
  Dimension m_lastViewPortDim;
  bool m_shareOnlyApp;
  Region m_appRegion;
  Region m_prevAppRegion;
  LocalMutex m_viewPortMut;

  UpdateKeeper* m_updateKeeper;

  FrameBuffer m_frameBuffer;
  Desktop* m_desktop;

  CursorUpdates m_cursorUpdates;

  EncodeOptions m_newEncodeOptions;
  LocalMutex m_newEncodeOptionsLocker;

  PixelFormat m_newPixelFormat;
  LocalMutex m_newPixelFormatLocker;

  bool m_setColorMapEntr;
  bool m_videoFrozen;
  Region m_prevVideoRegion;
  LocalMutex m_vidFreezeLocMut;

  Region m_losslessDirty;
  Region m_losslessClean;

  RfbOutputGate* m_output;
  PixelConverter m_pixelConverter;
  EncoderStore m_enbox;
  int m_id;
};

