#pragma once
#include "Utils/CommonHeader.h"
#include<vector>
#include "Utils/StringVector.h"

class WindowFinder
{
public:
	static std::vector<HWND> findWindowsByClass(StringVector classNames);
	static HWND findFirstWindowByName(const StringStorage windowName);

protected:
	static BOOL CALLBACK findWindowsByClassFunc(HWND hwnd, LPARAM lParam);
	static BOOL CALLBACK findWindowsByNameFunc(HWND hwnd, LPARAM lParam);

};

