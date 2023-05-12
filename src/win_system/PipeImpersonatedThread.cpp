#include "PipeImpersonatedThread.h"
#include "Environment.h"

PipeImpersonatedThread::PipeImpersonatedThread(HANDLE pipeHandle)
    :m_pipeHandle(pipeHandle),
    m_success(false)
{
}

PipeImpersonatedThread::~PipeImpersonatedThread()
{
    terminate();
    wait();
}

void PipeImpersonatedThread::waitUntilImpersonated()
{
    m_threadSleeper.waitForEvent();
}

bool PipeImpersonatedThread::getImpersonationSuccess()
{
    return m_success;
}

void PipeImpersonatedThread::getFaultReason(StringStorage* faultReason)
{
    *faultReason = m_faultReason;
}

void PipeImpersonatedThread::execute()
{
    m_success = ImpersonateNamedPipeClient(m_pipeHandle) != 0;
    if (!m_success) {
        Environment::getErrStr(&m_faultReason);
    }
    m_impersonationReadyEvent.notify();
    while (!isTerminating()) {
        m_threadSleeper.waitForEvent();
    }
    RevertToSelf(); //ToTest
}

void PipeImpersonatedThread::onTerminate()
{
    m_threadSleeper.notify();
}
