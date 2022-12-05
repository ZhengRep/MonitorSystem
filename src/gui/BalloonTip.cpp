#include "BalloonTip.h"

BalloonTip::BalloonTip(const TCHAR* text, const TCHAR* caption)
{
    setText(text);
    setTitle(caption);
}

BalloonTip::BalloonTip()
{
    setText(_T(""));
    setTitle(_T(""));
}

BalloonTip::~BalloonTip()
{
}

void BalloonTip::showTooltip(Control* control)
{
    MessageBox(control->getWindow(), m_text.getString(), m_title.getString(), MB_OK | MB_ICONWARNING);
}

void BalloonTip::setText(const TCHAR* text)
{
    m_text.setString(text);
}

void BalloonTip::setTitle(const TCHAR* caption)
{
    m_title.setString(caption);
}

void BalloonTip::getText(StringStorage* text) const
{
    text->setString(m_text.getString());
}

void BalloonTip::getTitle(StringStorage* title) const
{
    title->setString(m_title.getString());
}

void BalloonTip::setIconType(int iconType)
{

}

int BalloonTip::getIconType() const
{
    return 0;
}
