#include "TextBox.h"

TextBox::~TextBox()
{
}

size_t TextBox::getTextLengthLimit()
{
    return SendMessage(m_hwnd, EM_GETLIMITTEXT, 0, 0);
}

void TextBox::setTextLengthLimit(size_t limit)
{
    SendMessage(m_hwnd, EM_SETLIMITTEXT, limit, 0);
}

int TextBox::getCurrentLineIndex()
{
    return (int)SendMessage(m_hwnd, EM_LINEINDEX, -1, 0);
}

int TextBox::getLineCount()
{
    return (int)SendMessage(m_hwnd, EM_GETLINECOUNT, 0, 0);
}

int TextBox::getCaretPos()
{
    return 0;
}

void TextBox::setCarePos(int h, int v)
{
    SendMessage(m_hwnd, EM_LINESCROLL, h, v);
}

void TextBox::selectText(size_t startPos, size_t endPos)
{
    SendMessage(getWindow(), EM_SETSEL, startPos, endPos);
}

void TextBox::showBalloonTip(Tooltip* tip)
{
    tip->showTooltip(this);
}
