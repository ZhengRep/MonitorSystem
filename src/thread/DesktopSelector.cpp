#include "DesktopSelector.h"
#include<vector>

HDESK DesktopSelector::getInputDesktop()
{
    return OpenInputDesktop(0, TRUE, DESKTOP_CREATEMENU | 
        DESKTOP_CREATEWINDOW | 
        DESKTOP_ENUMERATE | 
        DESKTOP_HOOKCONTROL |
        DESKTOP_WRITEOBJECTS | 
        DESKTOP_READOBJECTS | 
        DESKTOP_READOBJECTS | 
        DESKTOP_SWITCHDESKTOP
        | GENERIC_WRITE);
}

HDESK DesktopSelector::getDesktop(const StringStorage* name)
{
    return OpenDesktop(name->getString(), 0, TRUE,
        DESKTOP_CREATEMENU |
        DESKTOP_CREATEWINDOW |
        DESKTOP_ENUMERATE |
        DESKTOP_HOOKCONTROL |
        DESKTOP_WRITEOBJECTS |
        DESKTOP_READOBJECTS |
        DESKTOP_SWITCHDESKTOP |
        GENERIC_WRITE);
}

bool DesktopSelector::closeDesktop(HDESK hdesk)
{
    return CloseDesktop(hdesk) != 0;
}

bool DesktopSelector::setDesktopToCurrentThread(HDESK newDesktop)
{
    return SetThreadDesktop(newDesktop) != 0;
}

bool DesktopSelector::selectDesktop(const StringStorage* name)
{
    HDESK desktop;
    if (name) {
        desktop = getDesktop(name);
    }
    else {
        desktop = getInputDesktop();
    }
    bool result = setDesktopToCurrentThread(desktop) != 0;
    closeDesktop(desktop);
    return result;
}

bool DesktopSelector::getCurrentDesktopName(StringStorage* desktopName)
{
    HDESK inputDesktop = getInputDesktop();
    bool result = getDesktopName(inputDesktop, desktopName);
    closeDesktop(inputDesktop);
    return result;
}

bool DesktopSelector::getThreadDesktopName(StringStorage* desktopName)
{
    return getDesktopName(GetThreadDesktop(GetCurrentThreadId()), desktopName);
}

bool DesktopSelector::getDesktopName(HDESK desktop, StringStorage* desktopName)
{
    desktopName->setString(_T(""));

    DWORD nameLength = 0;
    GetUserObjectInformation(desktop, UOI_NAME, 0, 0, &nameLength);
    if (nameLength != 0) {
        std::vector<TCHAR> name(nameLength);
        bool result = !!GetUserObjectInformation(desktop, UOI_NAME, &name[0], nameLength, 0);
        if (result) {
            desktopName->setString(&name[0]);
            return true;
        }
    }
    return false;
}
