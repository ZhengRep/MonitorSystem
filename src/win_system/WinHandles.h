#pragma once
#include "utils/CommonHeader.h"

class WinHandles
{
public:
  static HANDLE assignHandleFor(HANDLE hSource, HANDLE hTargetProc, bool neededToCloseSource, bool keepCloseRight);
  static HANDLE assignHandleFor(HANDLE hSource, unsigned int procId, bool neededToCloseSource, bool keepCloseRight);

private:
  WinHandles();
};

