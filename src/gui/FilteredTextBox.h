#pragma once
#include "TextBox.h"
#include "BalloonTip.h"
#include "StringFilter.h"

class FilteredTextBox: public TextBox
{
public:
	FilteredTextBox();
	~FilteredTextBox();

	//Override Control :: setWindow method
	void setWindow(HWND hwnd);
	void setText(TCHAR* text);
	void setErrorBalloonTip(BalloonTip* tip);
	void setStringFilter(StringFilter* filter);

	LRESULT makeCheck();

protected:
	virtual bool isStringValid(const TCHAR* string);
	virtual LRESULT onKeyDown(WPARAM code, LPARAM params);

	static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	LONG_PTR m_oldWindowProc;
	StringStorage m_text;
	BalloonTip* m_tip;
	StringFilter* m_filter;
};

