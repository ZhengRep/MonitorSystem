#include "Thread.h"
#include "AutoLock.h"
#include "Utils/Exception.h"

Thread::Thread()
    :m_terminated(false), m_active(false)
{
    m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProc, (LPVOID)this, CREATE_SUSPENDED, (LPDWORD)&m_threadID);
}

Thread::~Thread()
{
    CloseHandle(m_hThread);
}

bool Thread::wait()
{
    if (m_active) {
        return WaitForSingleObject(m_hThread, INFINITE) != WAIT_FAILED;
    }
    else {
        return true;
    }
}

bool Thread::suspend()
{
    m_active = !(SuspendThread(m_hThread) != -1);
    return !m_active;
}

bool Thread::resume()
{
    m_active = ResumeThread(m_hThread) != -1;
    return m_active;
}

void Thread::terminate()
{
    m_terminated= true;
    onTerminate();
}

bool Thread::isActive() const
{
    return m_active;
}

DWORD Thread::getThreadId() const
{
    return m_threadID;
}

bool Thread::setPriority(THREAD_PRIORITY value)
{
    int priority;

    switch (value)
    {
    case PRIORITY_IDLE:
        priority = THREAD_PRIORITY_IDLE;
        break;
    case PRIORITY_LOWEST:
        priority = THREAD_PRIORITY_LOWEST;
        break;
    case PRIORITY_BELOW_NORMAL:
        priority = THREAD_PRIORITY_BELOW_NORMAL;
        break;
    case PRIORITY_NORMAL:
        priority = THREAD_PRIORITY_NORMAL;
        break;
    case PRIORITY_ABOVE_NORMAL:
        priority = THREAD_PRIORITY_ABOVE_NORMAL;
        break;
    case PRIORITY_HIGHEST:
        priority = THREAD_PRIORITY_HIGHEST;
        break;
    case PRIORITY_TIME_CRITICAL:
        priority = THREAD_PRIORITY_TIME_CRITICAL;
        break;
    default:
        priority = THREAD_PRIORITY_NORMAL;
    }

    return SetThreadPriority(m_hThread, priority) != 0;
}

void Thread::sleep(DWORD millis)
{
    Sleep(millis);
}

void Thread::yield()
{
    SwitchToThread();
}

bool Thread::isTerminating()
{
    return m_terminated;
}

void Thread::onTerminate()
{
}

DWORD __stdcall Thread::threadProc(LPVOID pThread)
{
    Thread* _this = (Thread*)pThread;
    try {
        _this->initByDerived();
        _this->execute();
    }
    catch (Exception&) {

    }
    _this->m_active = false;
    return 0;
}

void Thread::initByDerived()
{
    //If is needed this function will be inherited by a derived class
}
