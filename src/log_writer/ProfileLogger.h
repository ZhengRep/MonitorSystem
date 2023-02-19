#pragma once

#include "Utils/DateTime.h"
#include<vector>
#include<map>
#include "thread/LocalMutex.h"

struct ProcessorTimes {
	double process;
	double kernel;
	ULONG64 cycle;
	DateTime wall;
};

//class for aquiring processor load metrics
class ProfileLogger
{
public:
	ProfileLogger(double rate = 5.0)
	: m_dropRate(rate)
	{
		m_lastDrop = DateTime::now();
	}

	~ProfileLogger();
	ProcessorTimes checkPoint(const TCHAR* tag);
	std::vector<std::vector<TCHAR>> dropStat();

private:
	LocalMutex m_mapMut;
	std::map < const TCHAR*, std::vector<ProcessorTimes>> m_checkPoints;
	ProcessorTimes m_last;
	double m_dropRate; //time interval in seconds to log statistics
	DateTime m_lastDrop;
};

