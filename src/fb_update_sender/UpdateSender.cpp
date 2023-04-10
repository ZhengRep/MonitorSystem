#include "UpdateSender.h"
#include "rfb/VendorDefs.h"
#include "rfb/MsgDefs.h"
#include <vector>
#include <algorithm>
#include "Utils/Exception.h"
#include "UpdSenderMsgDefs.h"
#include "rfb_utils/ClipboardExchange.h"

UpdateSender::UpdateSender(RfbCodeRegistrator* codeRegtor, UpdateRequestListener* updReqListener, SenderControlInformationInterface* senderControlInformation, RfbOutputGate* output, int id, Desktop* desktop, LogWriter* log)
  : m_updReqListener(updReqListener),
    m_desktop(desktop),
    m_senderControlInformation(senderControlInformation),
    m_busy(false),
    m_incrUpdIsReq(false),
    m_fullUpdIsReq(false),
    m_setColorMapEntr(false),
    m_output(output),
    m_enbox(&m_pixelConverter, m_output),
    m_id(id),
    m_videoFrozen(false),
    m_shareOnlyApp(false),
    m_log(log),
    m_cursorUpdates(log)
{
  m_updateKeeper = new UpdateKeeper(&Rect());

  //Capabilities
  codeRegtor->addEncCap(EncodingDefs::COPYRECT, VendorDefs::STANDARD, EncodingDefs::SIG_COPYRECT);
  codeRegtor->addEncCap(EncodingDefs::HEXTILE, VendorDefs::STANDARD, EncodingDefs::SIG_HEXTILE);
  codeRegtor->addEncCap(EncodingDefs::TIGHT, VendorDefs::VNC, EncodingDefs::SIG_TIGHT);

  codeRegtor->addEncCap(PseudoEncDefs::COMPR_LEVEL_0, VendorDefs::VNC, PseudoEncDefs::SIG_COMPR_LEVEL); //verdorSig nameSig
  codeRegtor->addEncCap(PseudoEncDefs::QUALITY_LEVEL_0, VendorDefs::VNC, PseudoEncDefs::SIG_QUALITY_LEVEL);
  codeRegtor->addEncCap(PseudoEncDefs::RICH_CURSOR, VendorDefs::VNC, PseudoEncDefs::SIG_RICH_CURSOR);
  codeRegtor->addEncCap(PseudoEncDefs::POINTER_POS, VendorDefs::VNC, PseudoEncDefs::SIG_POINTER_POS);

  codeRegtor->addClToSrvCap(UpdSenderClientMsgDefs::RFB_VIDEO_FREEZE, VendorDefs::VNC, UpdSenderClientMsgDefs::RFB_VIDEO_FREEZE_SIG);

  // Request codes
  codeRegtor->regCode(UpdSenderClientMsgDefs::RFB_VIDEO_FREEZE, this);
  codeRegtor->regCode(ClientMsgDefs::FB_UPDATE_REQUEST, this);
  codeRegtor->regCode(ClientMsgDefs::SET_PIXEL_FORMAT, this);
  codeRegtor->regCode(ClientMsgDefs::SET_ENCODINGS, this);

  resume();
}

UpdateSender::~UpdateSender()
{
  terminate();
  wait();
  delete m_updateKeeper;
}

void UpdateSender::init(const Dimension* viewPortDimension, const PixelFormat* pf)
{
  setClientPixelFormat(pf, false);
  {
    AutoLock al(&m_viewPortMut);
    m_clientDim = *viewPortDimension;
  }
  m_lastViewPortDim = *viewPortDimension;
  m_updateKeeper->setBorderRect(&viewPortDimension->getRect());
}

void UpdateSender::newUpdates(const UpdateContainer* updateContainer, const CursorShape* cursorShape)
{
  m_log->debug(_T("New updates passed to client #%d"), m_id);
  addUpdateContainer(updateContainer);

  m_cursorUpdates.updateCursorShape(cursorShape);
  {
    AutoLock al(&m_reqRectLocMut);
    m_busy = true;
    m_newUpdatesEvent.notify();
  }
  m_log->debug(_T("Client #%d is waking up"), m_id);
}

void UpdateSender::blockCursorPosSending()
{
  m_cursorUpdates.blockCursorPosSending();
}

Rect UpdateSender::getViewPort()
{
  AutoLock al(&m_viewPortMut);
  return m_viewPort;
}

bool UpdateSender::clientIsReady()
{
  AutoLock al(&m_reqRectLocMut);
  return (m_incrUpdIsReq || m_fullUpdIsReq) && !m_busy;
}

void UpdateSender::onRequest(UINT32 reqCode, RfbInputGate* input)
{
  //UpdateSender internal dispatcher
  switch (reqCode) {
  case ClientMsgDefs::FB_UPDATE_REQUEST:
    readUpdateRequest(input);
    break;
  case ClientMsgDefs::SET_PIXEL_FORMAT:
    readSetPixelFormat(input);
    break;
  case ClientMsgDefs::SET_ENCODINGS:
    readSetEncodings(input);
    break;
  case UpdSenderClientMsgDefs::RFB_VIDEO_FREEZE:
    readVideoFreeze(input);
    break;
  default:
    StringStorage errMess;
    errMess.format(_T("Unknown %d protocol code received"), (int)reqCode);
    throw Exception(errMess.getString());
    break;
  }
}

void UpdateSender::readUpdateRequest(RfbInputGate* io)
{
}

void UpdateSender::readSetPixelFormat(RfbInputGate* io)
{
}

void UpdateSender::readSetEncodings(RfbInputGate* io)
{
}

void UpdateSender::readVideoFreeze(RfbInputGate* io)
{
}

void UpdateSender::addUpdateContainer(const UpdateContainer* updateContainer)
{
  UpdateContainer updCont = *updateContainer;

  Rect viewPort = getViewPort();

  updCont.videoRegion.translate(-viewPort.left, -viewPort.top);
  updCont.changedRegion.translate(-viewPort.left, -viewPort.top);
  updCont.copiedRegion.translate(-viewPort.left, -viewPort.top);
  updCont.copySrc.move(-viewPort.left, -viewPort.top);

  m_updateKeeper->addUpdateContainer(&updCont);
}

void UpdateSender::execute()
{
}

void UpdateSender::onTerminate()
{
  m_newUpdatesEvent.notify();
}

void UpdateSender::checkCursorPos(UpdateContainer* updCont, const Rect* viewPort)
{
}

void UpdateSender::setVideoFrozen(bool value)
{
}

void UpdateSender::getVideoFrozen()
{
}

void UpdateSender::sendUpdate()
{
}

bool UpdateSender::extractReqRegions(Region* incrReqReg, Region* fullReqReg, bool* incrUpdIsReq, bool* fullUpdIsReq, DateTime* reqTimePoint)
{
  return false;
}

void UpdateSender::extractUpdates(UpdateContainer* updCont)
{
}

void UpdateSender::cropUpdContForReqRegions(UpdateContainer* updCont, const Region* incrReqReg, const Region* fullReqReg)
{
}

void UpdateSender::inscribeCopiedRegionToReqRegion(UpdateContainer* updCont, const Region* requestRegion)
{
}

void UpdateSender::selectEncoder(EncodeOptions* encodeOptions)
{
}

void UpdateSender::updateFrameBuffer(UpdateContainer* updCont, bool shareOnlyApp, const Region* prevSharedRegion, const Region* shareAppRegion)
{
}

bool UpdateSender::updateViewPort(Rect* outNewViewPort, bool* shareApp, Region* prevShareAppRegion, Region* newShareAppRegion)
{
  return false;
}

void UpdateSender::sendPalette(PixelFormat* pf)
{
}

void UpdateSender::setClientPixelFormat(const PixelFormat* pf, bool clrMapEntries)
{
}

void UpdateSender::sendRectHeader(const Rect* rect, INT32 encodingType)
{
  m_output->writeUInt16(rect->left);
  m_output->writeUInt16(rect->top);
  m_output->writeUInt16(rect->getWidth());
  m_output->writeUInt16(rect->getHeight());
  m_output->writeInt32(encodingType);
}

void UpdateSender::sendRectHeader(UINT16 x, UINT16 y, UINT16 w, UINT16 h, INT32 encodingType)
{
  m_output->writeUInt16(x);
  m_output->writeUInt16(y);
  m_output->writeUInt16(w);
  m_output->writeUInt16(h);
  m_output->writeInt32(encodingType);
}

void UpdateSender::sendNewFBSize(Dimension* dim)
{
  // Header
  m_output->writeUInt8(ServerMsgDefs::FB_UPDATE); // message type
  m_output->writeUInt8(0); // padding
  m_output->writeUInt16(1); // one rectangle

  Rect r(dim->width, dim->height);
  sendRectHeader(&r, PseudoEncDefs::DESKTOP_SIZE);
}

void UpdateSender::sendFbInClientDim(const EncodeOptions* encodeOptions, const FrameBuffer* fb, const Dimension* dim, const PixelFormat* pf)
{

}

void UpdateSender::sendCursorShapeUpdate(const PixelFormat* fmt, const CursorShape* cursorShape)
{
}

void UpdateSender::sendCursorPosUpdate()
{
}

void UpdateSender::sendCopyRect(const std::vector<Rect>* rects, const Point* source)
{
}

void UpdateSender::sendRectangles(Encoder* encoder, const std::vector<Rect>* rects, const FrameBuffer* frameBuffer, const EncodeOptions* encodeOptions)
{
}

void UpdateSender::paintBlack(FrameBuffer* frameBuffer, const Region* blackRegion)
{
}

void UpdateSender::splitRegion(Encoder* encoder, const Region* region, std::vector<Rect>* rects, const FrameBuffer* frameBuffer, const EncodeOptions* encodeOptions)
{
}

Region UpdateSender::takePartFromRegion(Region* reg, int area)
{
  return Region();
}

int UpdateSender::calcAreas(std::vector<Rect> rects)
{
  return 0;
}
