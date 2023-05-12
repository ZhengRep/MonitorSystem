#pragma once
#include "utils/CommonHeader.h"
#include "Screen.h"
#include<iostream>

#ifndef CAPTUREBLT
#define CAPTUREBLT (DWORD)0x40000000
#endif

class DibSection
{
 public:
  DibSection(const PixelFormat* pf, const Dimension* dim, HWND compatibleWin = 0);
  virtual ~DibSection();
  void setTargetDC(HDC targetDC);
  void* getBuffer();
  void blitToDibSection(const Rect* rect);
  void blitTransparentToDibSection(const Rect* rect);
  void blitFromDibSection(const Rect* rect);
  void stretchFromDibSection(const Rect* srcRect, const Rect* dstRect);

 private:
  void openDIBSection(const PixelFormat* pf, const Dimension* dim, HWND compatibleWin);
  void closeDIBSection();
  void releaseTargetDC();
  void blitToDibSection(const Rect* rect, DWORD flags);
  void blitFromDibSection(const Rect* rect, DWORD flags);
  void stretchFromDibSection(const Rect* srcRect, const Rect* dstRect, DWORD flags);
  void setupBMIStruct(BITMAPINFO* pBmi, const PixelFormat* pf, const Dimension* dim);

  bool m_isOwnTargetDC;
  HDC m_targetDC;
  HDC m_memDC;
  HBITMAP m_hbmOld;
  HBITMAP m_hbmDIB;

  int m_srcOffsetX;
  int m_srcOffsetY;
  void* m_buffer;
  Screen m_screen;



};

