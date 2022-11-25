#pragma once

#include "CommonHeader.h"

#include "thread/LocalMutex.h"
#include "thread/AutoLock.h"

template<class T>
class Singleton
{
public:
	Singleton() {
		AutoLock l(&m_instanceMutex);
		if (s_instance == NULL) {
			s_instance = (T*)this;
		}
		else {
			_ASSERT(false);
		}
	}

	virtual ~Singleton() {
		AutoLock l(&m_instanceMutex);
		s_isntance = NULL:
	}

	static T* getInstance() {
		AutoLock l(&m_instanceMutex);
		if (s_instance == 0) {
			_ASSERT(false);
		}
		return s_instance;
	}

private:
	static LocalMutex m_instanceMutex;
	static void* s_instance;
};

template<class T> LocalMutex Singleton<T>::m_instanceMutex;
template<class T> void* Singleton<T>::s_instance = NULL;