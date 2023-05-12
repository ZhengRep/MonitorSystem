#pragma once
#include "utils/CommonHeader.h"
#include <TlHelp32.h>

//Retrieves information of the parent process
class ParentProcess
{
public:
  ParentProcess();
  virtual ~ParentProcess();
  bool isAlive();

private:
  void init();
  bool getProcessEntry(PROCESSENTRY32* peOut, DWORD pid, HANDLE hSnapshot);
  PROCESSENTRY32 m_parentPe;
  bool m_isInitialized;
};

