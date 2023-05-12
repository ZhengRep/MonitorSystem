#pragma once
#include "rfb/CursorShape.h"
#include "desktop/UpdateContainer.h"
#include "utils/DateTime.h"
#include "thread/LocalMutex.h"
#include "log_writer/LogWriter.h"
#include "rfb_utils/EncodeOptions.h"

class CursorUpdates
{
public:
  CursorUpdates(LogWriter* log);
  virtual ~CursorUpdates();

  void update(const EncodeOptions* encodeOptions, UpdateContainer* updCont, bool fullRegReq, const Rect* viewPort,
    bool shareOnlyApp, const Region* shareAppRegion, FrameBuffer* fb, CursorShape* cursorShape);

  void restoreFrameBuffer(FrameBuffer* fb);

  Point getCurPos();
  Rect getBackgroundRect();
  void blockCursorPosSending();
  void updateCursorShape(const CursorShape* curShape);

private:
  void extractCursorShape(CursorShape* curShape);
  bool checkCursorPos(UpdateContainer* updCont, const Rect* viewPort, bool curPosBlockingIsIgnored);
  void drawCursor(UpdateContainer* updCont, FrameBuffer* fb);
  bool isCursorPosBlocked();

  Point m_cursorPos;
  DateTime m_blockCurPosTime;
  CursorShape m_cursorShape;
  FrameBuffer m_shapeBackground;
  Point m_backgroundPos;
  LocalMutex m_curPosLocMut;
  bool m_isDrawCursorMethod;
  LogWriter* m_log;
};

