#pragma once
class EncodingDefs
{
public:
  static const int RAW = 0;
  static const int COPYRECT = 1;
  static const int RRE = 2;
  static const int HEXTILE = 5;
  static const int TIGHT = 7;
  static const int ZRLE = 16;

  static const char* const SIG_RAW;
  static const char* const SIG_COPYRECT;
  static const char* const SIG_RRE;
  static const char* const SIG_HEXTILE;
  static const char* const SIG_TIGHT;
  static const char* const SIG_ZRLE;
};

class PseudoEncDefs
{
public:
  static const int COMPR_LEVEL_0 = -256;
  static const int COMPR_LEVEL_1 = -255;
  static const int COMPR_LEVEL_2 = -254;
  static const int COMPR_LEVEL_3 = -253;
  static const int COMPR_LEVEL_4 = -252;
  static const int COMPR_LEVEL_5 = -251;
  static const int COMPR_LEVEL_6 = -250;
  static const int COMPR_LEVEL_7 = -249;
  static const int COMPR_LEVEL_8 = -248;
  static const int COMPR_LEVEL_9 = -247;

  static const int X_CURSOR = -240;
  static const int RICH_CURSOR = -239;
  static const int POINTER_POS = -232;

  static const int LAST_RECT = -224;
  static const int DESKTOP_SIZE = -223;

  static const int QUALITY_LEVEL_0 = -32;
  static const int QUALITY_LEVEL_1 = -31;
  static const int QUALITY_LEVEL_2 = -30;
  static const int QUALITY_LEVEL_3 = -29;
  static const int QUALITY_LEVEL_4 = -28;
  static const int QUALITY_LEVEL_5 = -27;
  static const int QUALITY_LEVEL_6 = -26;
  static const int QUALITY_LEVEL_7 = -25;
  static const int QUALITY_LEVEL_8 = -24;
  static const int QUALITY_LEVEL_9 = -23;

  static const char* const SIG_COMPR_LEVEL;
  static const char* const SIG_X_CURSOR;
  static const char* const SIG_RICH_CURSOR;
  static const char* const SIG_POINTER_POS;
  static const char* const SIG_LAST_RECT;
  static const char* const SIG_DESKTOP_SIZE;
  static const char* const SIG_QUALITY_LEVEL;
};

