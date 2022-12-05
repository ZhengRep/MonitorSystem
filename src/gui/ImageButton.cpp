#include "ImageButton.h"

ImageButton::ImageButton()
{
}

ImageButton::~ImageButton()
{
}

void ImageButton::drawItem(LPDRAWITEMSTRUCT dis)
{
}

void ImageButton::setWindow(HWND hwnd)
{
}

void ImageButton::setIcon(HICON* icon, int width, int height)
{
}

void ImageButton::calcRect(RECT* buttonRect, bool isButtonPressed, DWORD textWidth, DWORD textHeight, DWORD imageWidth, DWORD imageHeight, RECT* textRect, RECT* imageRect)
{
}

void ImageButton::drawIcon(HDC* dc, RECT* imageRect, bool isPressed, bool isDisabled)
{
}

LRESULT ImageButton::wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}
