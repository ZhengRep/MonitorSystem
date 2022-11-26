#include "AutoLock.h"

AutoLock::AutoLock(Lockable* locker)
	:m_locker(locker) //this is equal mean
{
	m_locker->lock();
}

AutoLock::~AutoLock()
{
	m_locker->unlock();
}
