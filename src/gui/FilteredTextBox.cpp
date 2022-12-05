#include "FilteredTextBox.h"

FilteredTextBox::FilteredTextBox()
    :m_oldWindowProc(NULL), m_tip(NULL)
{
    m_text.setString(_T(""));
}

FilteredTextBox::~FilteredTextBox()
{
}

void FilteredTextBox::setWindow(HWND hwnd)
{
    if (hwnd) {
        m_oldWindowProc = SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)&windowProc);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
    }
    else {
        m_oldWindowProc = NULL;
    }
    Control::setWindow(hwnd);
}

void FilteredTextBox::setText(TCHAR* text)
{
    m_text.setString(text);
    m_tip->setText(text);
}

void FilteredTextBox::setErrorBalloonTip(BalloonTip* tip)
{
    m_tip = tip;
}

void FilteredTextBox::setStringFilter(StringFilter* filter)
{
    m_filter = filter;
}

LRESULT FilteredTextBox::makeCheck()
{
    StringStorage updatedText;
    TextBox::getText(&updatedText);
    if (isStringValid(updatedText.getString())) {
        m_text = updatedText;
    }
    else {
        if (m_tip) {
            showBalloonTip(m_tip);
            TextBox::setText(m_text.getString());
            TextBox::selectText(m_text.getLength(), m_text.getLength());
        }
        else{

        }
        return -1;
    }
    return 0;
}

bool FilteredTextBox::isStringValid(const TCHAR* string)
{
    if (m_filter) {
        return m_filter->isStringCorrect(string);
    }
    return true;
}

LRESULT FilteredTextBox::onKeyDown(WPARAM code, LPARAM params)
{
    return makeCheck();
}

LRESULT FilteredTextBox::windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    FilteredTextBox* _this = (FilteredTextBox*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (_this == NULL) {
        return FALSE;
    }
    switch (uMsg) {
    case WM_CHAR:
        LRESULT result = CallWindowProc((WNDPROC)_this->m_oldWindowProc, hwnd, uMsg, wParam, lParam);
        return _this->onKeyDown(wParam, lParam);
    }
    return CallWindowProc((WNDPROC)_this->m_oldWindowProc, hwnd, uMsg, wParam, lParam);
}
