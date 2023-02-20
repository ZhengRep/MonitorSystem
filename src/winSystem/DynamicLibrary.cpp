#include "DynamicLibrary.h"

DynamicLibrary::DynamicLibrary(const TCHAR* fileName) throw(Exception)
    :m_module(0)
{
    init(fileName);
}

DynamicLibrary::DynamicLibrary()
    :m_module(0)
{
}

DynamicLibrary::~DynamicLibrary()
{
    if (m_module) {
        FreeLibrary(m_module);
    }
}

void DynamicLibrary::init(const TCHAR* fileName)
{
    m_module = LoadLibrary(fileName);
    if (m_module == 0) {
        StringStorage errMsg;
        errMsg.format(_T("%s library not found."), fileName);
        throw Exception(errMsg.getString());
    }
}

FARPROC DynamicLibrary::getProcAddress(const char* procName)
{
    _ASSERT(m_module != 0);
    return ::GetProcAddress(m_module, procName);
}
