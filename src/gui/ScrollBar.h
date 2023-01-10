#pragma once
#include "Utils/CommonHeader.h"

class ScrollBar
{
public:
	ScrollBar();

	void setWindows(HWND hwnd);
	bool showVertScroll(bool show);
	bool showHorzScroll(bool show);
	void setHorzRange(int imin, int imax, int istep);
	void setVertRange(int imin, int imax, int istep);
	void setVertPos(int iPos);
	void setHorzPos(int iPos);
	void moveUpVert(int iPercent = 0);
	void moveDownVert(int iPercent = 0);
	void moveLeftHorz(int iPercent = 0);
	void moveRightHorz(int iPercent = 0);
	int getVertPos();
	int getHorzPos();
	int getVertivalSize();
	int getHorizontalSize();

	static const int SCROLL_STEP = 5;

protected:
	void enableVirtualSrcoll();
	void disableVirtualSrcoll();
	bool _showVertScroll(bool show);
	bool _showHorzScroll(bool show);

	HWND m_hwnd;
	int m_vPos, m_hPos;
	int m_vMin, m_vMax;
	int m_hMin, m_hMax;
	int m_vStep;
	int m_hStep;
	bool m_isVirtualScroll;
	bool m_isVVert;
	bool m_isVHorz;
	bool m_isVert;
	bool m_isHorz;

};

