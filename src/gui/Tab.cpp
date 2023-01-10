#include "Tab.h"

Tab::Tab()
    :m_dialog(NULL)
{
    m_caption.setString(_T(""));
}

Tab::Tab(BaseDialog* dialog, const TCHAR* caption)
    :m_dialog(dialog)
{
    m_caption.setString(caption);
}

void Tab::setCaption(const TCHAR* caption)
{
    m_caption.setString(caption);
}

const TCHAR* Tab::getCaption()
{
    return m_caption.getString();
}

void Tab::setDialog(BaseDialog* dialog)
{
    m_dialog = dialog;
}

BaseDialog* Tab::getDialog()
{
    return m_dialog;
}

bool Tab::isOk()
{
    return m_dialog != NULL;
}

void Tab::setVisible(bool visible)
{
    if (m_dialog) {
        m_dialog->getControl()->setVisiable(visible);
    }
}
