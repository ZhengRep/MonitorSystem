#pragma once
#include "utils/CommonHeader.h"
#include<vector>
#include "utils/StringVector.h"

class WindowFinder
{
public:
	static std::vector<HWND> findWindowsByClass(StringVector classNames);
	static HWND findFirstWindowByName(const StringStorage windowName);

protected:
	static BOOL CALLBACK findWindowsByClassFunc(HWND hwnd, LPARAM lParam);
	static BOOL CALLBACK findWindowsByNameFunc(HWND hwnd, LPARAM lParam);

};

