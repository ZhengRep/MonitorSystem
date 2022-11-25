#pragma once
#include "Utils/CommonHeader.h"
#include "Lockable.h"

/*
* Local mutex(cannot be used within separate process.
* @ramark local mutex uses Windows critical sections to implement lockable interface
*/
class LocalMutex: public Lockable
{
public:
	LocalMutex();
	virtual ~LocalMutex();
	//Inherited from Lockable
	virtual void lock();
	virtual void unlock();

private:
	CRITICAL_SECTION m_criticalSection;
};

