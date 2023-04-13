#include "AnonymousPipeFactory.h"
#include "SecurityAttributes.h"

AnonymousPipeFactory::AnonymousPipeFactory(unsigned int bufferSize, LogWriter* log)
  : m_bufferSize(bufferSize),
   m_log(log)
{
}

AnonymousPipeFactory::~AnonymousPipeFactory()
{
}

void AnonymousPipeFactory::generatePipes(AnonymousPipe** firstSide, bool firstSideIsInheritable, AnonymousPipe** secondSide, bool secondSideIsInheritable)
{
  HANDLE hFirstSideWrite = 0, hFirstSideRead = 0, 
    hSecondSideWrite = 0, hSecondSideRead = 0;

  SecurityAttributes secAttr;
  secAttr.setInheritable();

  try {
    if (CreatePipe(&hFirstSideRead, &hSecondSideWrite, secAttr.getSecurityAttributes(), m_bufferSize) == 0) {
      SystemException(_T("Cannot create anonymous pipe"));
    }
    if (CreatePipe(&hSecondSideRead, &hFirstSideWrite, secAttr.getSecurityAttributes(), m_bufferSize) == 0) {
      SystemException(_T("Cannot create anonymous pipe"));
    }
  } catch (...) {
    CloseHandle(hFirstSideWrite);
    CloseHandle(hFirstSideRead);
    CloseHandle(hSecondSideWrite);
    CloseHandle(hSecondSideRead);
    throw;
  }

  const TCHAR* errMess = _T("Cannot disable inheritance for anonymous pipe");
  if (!firstSideIsInheritable) {
    if (SetHandleInformation(hFirstSideWrite, HANDLE_FLAG_INHERIT, 0) == 0) {
      SystemException(errMess);
    }
    if (SetHandleInformation(hFirstSideRead, HANDLE_FLAG_INHERIT, 0) == 0) {
      SystemException(errMess);
    }
  }
  if (!secondSideIsInheritable) {
    if (SetHandleInformation(hSecondSideWrite, HANDLE_FLAG_INHERIT, 0) == 0) {
      SystemException(errMess);
    }
    if (SetHandleInformation(hSecondSideRead, HANDLE_FLAG_INHERIT, 0) == 0) {
      SystemException(errMess);
    }
  }

  *firstSide = new AnonymousPipe(hFirstSideWrite, hFirstSideRead, m_bufferSize, m_log);
  *secondSide = new AnonymousPipe(hSecondSideWrite, hSecondSideRead, m_bufferSize, m_log);
}
