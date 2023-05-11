#include "ClientInputHandler.h"
#include "rfb/MsgDefs.h"

ClientInputHandler::ClientInputHandler(RfbCodeRegistrator* codeRegtor, ClientInputEventListener* extEventListener, bool viewOnly)
  :m_extEventListener(extEventListener),
  m_viewOnly(viewOnly)
{
  codeRegtor->regCode(ClientMsgDefs::KEYBOARD_EVENT, this);
  codeRegtor->regCode(ClientMsgDefs::POINTER_EVENT, this);
}

ClientInputHandler::~ClientInputHandler()
{
}

void ClientInputHandler::onRequest(UINT32 reqCode, RfbInputGate* input)
{
  switch (reqCode) {
  case ClientMsgDefs::KEYBOARD_EVENT:
  {
    bool down = input->readUInt8() != 0;
    input->readUInt16(); // Pad
    UINT32 keyCode = input->readUInt32();
    if (!m_viewOnly) {
      m_extEventListener->onKeyboardEvent(keyCode, down);
    }
  }
  break;
  case ClientMsgDefs::POINTER_EVENT:
  {
    UINT8 buttonMask = input->readUInt8();
    UINT16 x = input->readUInt16();
    UINT16 y = input->readUInt16();
    if (!m_viewOnly) {
      m_extEventListener->onMouseEvent(x, y, buttonMask);
    }
  }
  break;
  default:
    StringStorage errMess;
    errMess.format(_T("Unknown %d protocol code received"), (int)reqCode);
    throw Exception(errMess.getString());
    break;
  }
}
