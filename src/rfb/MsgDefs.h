#pragma once
#include "Utils/inttypes.h"

class ClientMsgDefs
{
public:
  static const UINT32 SET_PIXEL_FORMAT = 0;
  static const UINT32 FIX_COLOR_MAP_ENTRIES = 1;
  static const UINT32 SET_ENCODINGS = 2;
  static const UINT32 FB_UPDATE_REQUEST = 3;
  static const UINT32 KEYBOARD_EVENT = 4;
  static const UINT32 POINTER_EVENT = 5;
  static const UINT32 CLIENT_CUT_TEXT = 6;
  static const UINT32 CLIENT_CUT_TEXT_UTF8 = 0xFC000200;
  static const UINT32 ENABLE_CUT_TEXT_UTF8 = 0xFC000201;
  static const UINT32 ECHO_REQUEST = 0xFC000300;
};

class ServerMsgDefs
{
public:
  static const UINT32 FB_UPDATE = 0;
  static const UINT32 SET_COLOR_MAP_ENTRIES = 1;
  static const UINT32 BELL = 2;
  static const UINT32 SERVER_CUT_TEXT = 3;
  static const UINT32 SERVER_CUT_TEXT_UTF8 = 0xFC000200;
  static const UINT32 ECHO_RESPONSE = 0xFC000300;
};

class Utf8CutTextDefs
{
public:
  static const char* const CLIENT_CUT_TEXT_UTF8_SIG;
  static const char* const SERVER_CUT_TEXT_UTF8_SIG;
  static const char* const ENABLE_CUT_TEXT_UTF8_SIG;
};
class EchoExtensionDefs
{
public:
  static const char* const ECHO_REQUEST_SIG;
  static const char* const ECHO_RESPONSE_SIG;
};

