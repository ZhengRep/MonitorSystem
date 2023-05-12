#pragma once

#include "utils/StringStorage.h"
#include "log_writer/LogWriter.h"

class Environment
{
public:
	static const int APPLICATION_DATA_SPECIAL_FOLDER = 0x0;
	static const int COMMON_APPLICATION_DATA_SPECIAL_FOLDER = 0x1;

public:
	Environment();
	~Environment();

	static void getErrStr(StringStorage* out);
	static void getErrStr(const TCHAR* specification, StringStorage* out);

	static bool getSpecialFolderPath(int specialFolderId, StringStorage* out);

	static bool getCurrentModulePath(StringStorage* out);
	static bool isItTheSamePathAsCurrent(unsigned int pId);

	//Sets full path to folder(without last directory separator character)
	static bool getCurrentModuleFolderPath(StringStorage* out);
	static bool getCurrentUserName(StringStorage* out, LogWriter* log);
	static bool getComputerName(StringStorage* out);
	static bool isWinNTFamily();
	static bool isWin2000();
	static bool isWinXP();
	// Returns true if run windows 2003 server.
	static bool isWin2003Server();
	// Returns true if run under Windows Vista or later
	static bool isVistaOrLater();

	static void simulateCtrlAltDel(LogWriter* log);
	//simulates the "Ctrl + alt + del" combination by using the "SAS" lib
	static void simulateCtrlAltDelUnderVista(LogWriter* log);
	static bool isAeroOn(LogWriter* log);

private:
	static void init();
	static OSVERSIONINFO m_osVerInfo;
};

