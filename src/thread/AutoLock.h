#pragma once
#include "Lockable.h"

class AutoLock
{
public:
	AutoLock(Lockable* locker);
	virtual ~AutoLock();

protected:
	Lockable* m_locker;
};

