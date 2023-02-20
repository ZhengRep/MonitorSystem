#include "SystemException.h"
#include <crtdbg.h>

SystemException::SystemException()
    : Exception(), m_errcode(GetLastError())
{
    createMessage(0, m_errcode);
}

SystemException::SystemException(int errcode)
    : Exception(), m_errcode(errcode)
{
    createMessage(0, m_errcode);
}

SystemException::SystemException(const TCHAR* userMessage)
    : Exception(), m_errcode(GetLastError())
{
    createMessage(userMessage, m_errcode);
}

SystemException::SystemException(const TCHAR* userMessage, int errcode)
    : Exception(), m_errcode(errcode)
{
    createMessage(userMessage, m_errcode);
}

SystemException::~SystemException()
{
}

int SystemException::getErrorCode() const
{
    return m_errcode;
}

const TCHAR* SystemException::getSystemErrorDescription() const
{
    return m_systemMessage.getString();
}

void SystemException::createMessage(const TCHAR* userMessage, int errCode)
{
    if (userMessage == 0 && errCode) {
        userMessage = _T("Thrown a system exception but the program cannot identify the corresponding system error.");
    }

    //Get description of windows specific error
    bool formatMessageOk = true;
    TCHAR buffer[1023 * 10] = { 0 };
    if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errCode, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
        (LPTSTR)&buffer[0], sizeof(buffer), NULL) == 0) {
        formatMessageOk = false;
    }

    StringStorage windowsErrorDescription(buffer);

    //Remove bad characters
    const TCHAR badCharacters[] = { 10, 13, _T('\n'), _T('\t') };

    windowsErrorDescription.removeChars(badCharacters, sizeof(badCharacters) / sizeof(TCHAR));

    if (windowsErrorDescription.endsWith(_T('.'))) {
        windowsErrorDescription.truncate(1);
    }

    // Create system error part of message.

    if (formatMessageOk) {
        m_systemMessage.format(_T("%s (error code %d)"),
            windowsErrorDescription.getString(),
            errCode);
    }
    else {
        m_systemMessage.format(_T("Error code %d"), errCode);
    }

    // Use user message if specified.

    if (errCode != 0) {
        if (userMessage == 0) {
            m_message = m_systemMessage;
        }
        else {
            m_message.format(_T("%s (system error: %s)"),
                userMessage,
                m_systemMessage.getString());
        }
    }
    else {
        m_message = userMessage;
        m_systemMessage = userMessage;
    }
}


