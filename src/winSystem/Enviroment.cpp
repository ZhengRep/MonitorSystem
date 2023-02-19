#include "Enviroment.h"

Environment::Environment()
{
}

Environment::~Environment()
{
}

void Environment::getErrStr(StringStorage* out)
{
}

void Environment::getErrStr(const TCHAR* specification, StringStorage* out)
{
}

bool Environment::getSpecialFolderPath(int specialFolderId, StringStorage* out)
{
    return false;
}

bool Environment::getCurrentModulePath(StringStorage* out)
{
    return false;
}

bool Environment::isItTheSamePathAsCurrent(unsigned int pId)
{
    return false;
}

bool Environment::getCurrentModuleFolderPath(StringStorage* out)
{
    return false;
}

bool Environment::getCurentUserName(StringStorage* out, LogWriter* log)
{
    return false;
}

bool Environment::getComputerName(StringStorage* out)
{
    return false;
}

bool Environment::isWinNTFamily()
{
    return false;
}

bool Environment::isWin2000()
{
    return false;
}

bool Environment::isWinXP()
{
    return false;
}

bool Environment::isWin2003Server()
{
    return false;
}

bool Environment::isVistaOrLater()
{
    return false;
}

void Environment::simulateCtrlAltDel(LogWriter* log)
{
}

void Environment::simulateCtrlAltDelUnderVista(LogWriter* log)
{
}

bool Environment::isAeroOn(LogWriter* log)
{
    return false;
}
