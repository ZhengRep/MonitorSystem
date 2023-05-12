#pragma once
#include "utils/CommonHeader.h"

class WindowMessageHandler
{
public:
	//This function must return ture if a message has been processed
	virtual bool processMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;
};