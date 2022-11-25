#pragma once
class Lockable
{
public:
	virtual ~Lockable(){ };

	//locks object
	virtual void Lock() = 0;
	virtual void UnLock() = 0;
};

