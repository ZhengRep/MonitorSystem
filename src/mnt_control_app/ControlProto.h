#pragma once
#include "utils/CommonHeader.h"

/**
 * Control protocol constants.
 *
 * @section Client requests.
 * Client to server control messages are request-reply oriented and have format
 * descripbed above:
 *
 * UINT32 command code.
 * UINT32 message body size (in bytes).
 * UINT32[] message body (byte array with length that equals to message body size).
 *
 * @section Server replies.
 * Server to client messages (eq replies to client requests) have following format:
 * UINT32 reply code (REPLY_OK - no error, REPLY_ERROR - error during request processing,
 * REPLY_AUTH_NEEDED - authorization needed for this request).
 * UINT8[] body (reply body with unknown length (i mean body is individual for every reply,
 * some replies can have no body).
 */
class ControlProto
{
public:
  static const UINT32 REPLY_OK = 0x0;

  static const UINT32 REPLY_ERROR = 0x1;

  static const UINT32 REPLY_AUTH_NEEDED = 0x2;

  static const UINT32 GET_CLIENT_LIST_MSG_ID = 0x4;

  static const UINT32 RELOAD_CONFIG_MSG_ID = 0x5;

  static const UINT32 DISCONNECT_ALL_CLIENTS_MSG_ID = 0x6;

  static const UINT32 SHUTDOWN_SERVER_MSG_ID = 0x7;

  static const UINT32 ADD_CLIENT_MSG_ID = 0x8;

  static const UINT32 CONNECT_TO_TCPDISP_MSG_ID = 0x9;

  static const UINT32 SET_CONFIG_MSG_ID = 0x10;

  static const UINT32 GET_SERVER_INFO_MSG_ID = 0x11;

  static const UINT32 GET_CONFIG_MSG_ID = 0x12;

  static const UINT32 AUTH_MSG_ID = 0x13;

  static const UINT32 GET_SHOW_TRAY_ICON_FLAG = 0x14;

  static const UINT32 UPDATE_MNTCONTROL_PROCESS_ID_MSG_ID = 0x15;

  static const UINT32 SHARE_PRIMARY_MSG_ID = 0x20;

  static const UINT32 SHARE_DISPLAY_MSG_ID = 0x21;

  static const UINT32 SHARE_WINDOW_MSG_ID = 0x22;

  static const UINT32 SHARE_RECT_MSG_ID = 0x23;

  static const UINT32 SHARE_FULL_MSG_ID = 0x24;

  static const UINT32 SHARE_APP_MSG_ID = 0x25;
};

