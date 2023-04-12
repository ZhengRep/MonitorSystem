#include "Environment.h"
#include "CtrlAltDelSimulator.h"
#include<ShlObj.h>
#include<crtdbg.h>
#include "AutoImpersonator.h"
#include "WTS.h"
#include "ProcessHandle.h"
#include "Shell.h"
#include "DynamicLibrary.h"
#include <vector>
#include <algorithm>

#pragma warning(disable: 4996)

OSVERSIONINFO Environment::m_osVerInfo = { 0 };
typedef VOID(WINAPI* SendSas)(BOOL asUser);
typedef HRESULT(WINAPI* DwmIsCompositionEnabled)(BOOL* pfEnabled);

Environment::Environment()
{
    
}

Environment::~Environment()
{
}

void Environment::getErrStr(StringStorage* out)
{
    DWORD errCode = GetLastError();
    TCHAR buffer[1024];

    // FIXME: Remove "new line" character from buffer.
    if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errCode,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPTSTR)&buffer[0], sizeof(buffer), NULL) == 0) {
        out->format(_T("<<Cannot get text error describing>> (%u)"), errCode);
    }
    else {
        out->format(_T("%s (%u)"), buffer, errCode);
    }
}

void Environment::getErrStr(const TCHAR* specification, StringStorage* out)
{
    StringStorage sysErrText;
    getErrStr(&sysErrText);
    out->format(_T("%s (%s)"), specification, sysErrText.getString());
}

bool Environment::getSpecialFolderPath(int specialFolderId, StringStorage* out)
{
    _ASSERT(out != NULL);

    int csidl = 0;
    switch (specialFolderId) {
    case APPLICATION_DATA_SPECIAL_FOLDER:
        csidl = CSIDL_APPDATA;
        break;
    case COMMON_APPLICATION_DATA_SPECIAL_FOLDER:
        csidl = CSIDL_COMMON_APPDATA;
        break;
    default:
        _ASSERT(FALSE);
        return false;
    }

    bool ret = false;
    TCHAR path[MAX_PATH + 1];
    if (SHGetSpecialFolderPath(NULL, &path[0], csidl, TRUE) == TRUE) {
        out->setString(&path[0]);
        ret = true;
    }
    return ret;
}

bool Environment::getCurrentModulePath(StringStorage* out)
{
    std::vector<TCHAR> buffer;
    DWORD size = MAX_PATH;

    while (true) {
        buffer.resize(size + 1);
        DWORD ret = GetModuleFileName(NULL, &buffer[0], size);
        if (ret == 0) {
            return false;
        }
        else if (ret == size || GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
            size += 128;
        }
        else {
            break;
        }
    }
    out->setString(&buffer[0]);
    return true;
}

bool Environment::isItTheSamePathAsCurrent(unsigned int pId)
{
    StringStorage currentModulePath, testedModulePath;
    ProcessHandle pHandle;
    pHandle.openProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, pId);
    pHandle.getProcessModulePath(&testedModulePath);
    getCurrentModulePath(&currentModulePath);

    return currentModulePath.isEqualTo(&testedModulePath);
}

bool Environment::getCurrentModuleFolderPath(StringStorage* out)
{
    if (!getCurrentModulePath(out)) {
        return false;
    }

    size_t lastPos = out->findLast(_T('\\'));
    if (lastPos != -1) {
        out->getSubstring(out, 0, max(lastPos - 1, 0));
    }
    return true;
}

bool Environment::getCurentUserName(StringStorage* out, LogWriter* log)
{
    return WTS::getCurrentUserName(out, log); //ToDo create WTS
}

bool Environment::getComputerName(StringStorage* out)
{
    TCHAR compName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD length = MAX_COMPUTERNAME_LENGTH + 1;
    if (GetComputerName(compName, &length) == 0) {
        return false;
    }
    out->setString(compName);
    out->toLowerCase();
    return true;
}

bool Environment::isWinNTFamily()
{
    init();
    return m_osVerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

bool Environment::isWin2000()
{
    init();
    return m_osVerInfo.dwMajorVersion == 5 && m_osVerInfo.dwMinorVersion == 0;
}

bool Environment::isWinXP()
{
    init();
    return ((m_osVerInfo.dwMajorVersion == 5) && (m_osVerInfo.dwMinorVersion == 1) && isWinNTFamily());
}

bool Environment::isWin2003Server()
{
    init();
    return ((m_osVerInfo.dwMajorVersion == 5) && (m_osVerInfo.dwMinorVersion == 2) && isWinNTFamily());
}

bool Environment::isVistaOrLater()
{
    init();
    return m_osVerInfo.dwMajorVersion >= 6;
}

void Environment::simulateCtrlAltDel(LogWriter* log)
{
    log->info(_T("Requested Ctrl+Alt+Del simulation"));

    // Are we running on Windows NT OS family?
    if (!isVistaOrLater() && isWinNTFamily()) {
        CtrlAltDelSimulator cadSim;
        cadSim.wait();
    }
}

void Environment::simulateCtrlAltDelUnderVista(LogWriter* log)
{
    log->info(_T("Requested Ctrl+Alt+Del simulation under Vista or later"));

    try {
        DynamicLibrary sasLib(_T("sas.dll"));
        SendSas sendSas = (SendSas)sasLib.getProcAddress("SendSAS");
        if (sendSas == 0) {
            throw Exception(_T("The SendSAS function has not been found"));
        }
        sendSas(FALSE); // Try only under service
    }
    catch (Exception& e) {
        log->error(_T("The simulateCtrlAltDelUnderVista() function failed: %s"), e.getMessage());
    }
}

bool Environment::isAeroOn(LogWriter* log)
{
    try {
        DynamicLibrary dwmLib(_T("Dwmapi.dll"));
        DwmIsCompositionEnabled dwmIsEnabled = (DwmIsCompositionEnabled)dwmLib.getProcAddress("DwmIsCompositionEnabled");
        if (dwmIsEnabled == 0) {
            throw Exception(_T("The DwmIsCompositionEnabled() has not been found in the Dwmapi.dll"));
        }
        BOOL result = FALSE;
        HRESULT dwmIsEnabledResult = dwmIsEnabled(&result);
        if (dwmIsEnabledResult != S_OK) {
            StringStorage errMess;
            errMess.format(_T("The DwmIsCompositionEnabled() error code is %d"), (int)dwmIsEnabledResult);
            throw Exception(_T(""));
        }
        return result != FALSE;
    }
    catch (Exception& e) {
        log->error(_T("The DwmIsCompositionEnabled() function failed: %s"), e.getMessage());
        throw;
    }
}

void Environment::init()
{
    if(m_osVerInfo.dwOSVersionInfoSize == 0) {
        m_osVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

        if (!GetVersionEx(&m_osVerInfo)) {
            m_osVerInfo.dwOSVersionInfoSize = 0;
        }
    }
}