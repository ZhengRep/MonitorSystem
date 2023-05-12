#include "WindowProcHolder.h"

WindowProcHolder::WindowProcHolder()
{
}

WindowProcHolder::~WindowProcHolder()
{
}

LRESULT WindowProcHolder::defWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WindowProcHolder* wph = (WindowProcHolder*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (!wph) {
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	bool useDefWndProc = false;
	LRESULT r = wph->windowProc(hWnd, uMsg, wParam, lParam, & useDefWndProc);
	if (r) {
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return r;
}
