#include "MntLogFilename.h"
#include "file_lib/File.h"
#include "win_system/Environment.h"
#include "mnt_server_app/NamingDefs.h"

void MntLogFilename::queryLogFileDirectory(bool forService, bool shareToAll, StringStorage* logFileDirectory)
{
  int specialFolderId = (shareToAll) ? Environment::COMMON_APPLICATION_DATA_SPECIAL_FOLDER : Environment::APPLICATION_DATA_SPECIAL_FOLDER;
  StringStorage specialFolder(_T(""));

  Environment::getSpecialFolderPath(specialFolderId, &specialFolder);
  logFileDirectory->format(_T("%s\\%s"), specialFolder.getString(), LogNames::LOG_DIR_NAME);
}
