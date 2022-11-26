#pragma once
class Lockable
{
public:
	virtual ~Lockable(){}

	//locks object
	virtual void lock() = 0;
	virtual void unlock() = 0;
};

