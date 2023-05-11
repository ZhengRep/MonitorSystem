#include "FolderListener.h"

FolderListener::FolderListener(const TCHAR* folderPath)
  : m_filesInfo(NULL), m_filesCount(0)
{
  m_folderPath.setString(folderPath);
}

FolderListener::~FolderListener()
{
  if (m_filesInfo != NULL) {
    delete[] m_filesInfo;
  }
}

const FileInfo* FolderListener::getFilesInfo() const
{
  return m_filesInfo;
}

UINT32 FolderListener::getFilesCount() const
{
  return m_filesCount;
}

bool FolderListener::list()
{
  StringStorage* fileNameList = NULL;
  m_filesCount = 0;
  bool listResult = true;

  if (!m_folderPath.isEmpty()) {
    File folder(m_folderPath.getString());
    listResult = folder.list(NULL, &m_filesCount);
  } else {
    File::listRoots(NULL, &m_filesCount);
  }

  if (!listResult) {
    return false;
  }

  fileNameList = new StringStorage[m_filesCount];

  if (m_filesInfo != NULL) {
    delete[] m_filesInfo;
    m_filesInfo = NULL;
  }

  m_filesInfo = new FileInfo[m_filesCount];

  if (!m_folderPath.isEmpty()) {
    File folder(m_folderPath.getString());
    folder.list(fileNameList, NULL);
  } else {
    File::listRoots(fileNameList, NULL);
  }

  for (UINT32 i = 0; i < m_filesCount; i++) {

    const TCHAR* fileName = fileNameList[i].getString();
    const TCHAR* folderName = m_folderPath.getString();

    File file(folderName, fileName);
    FileInfo fileInfo(&file);

    m_filesInfo[i] = fileInfo;

    // All files in root folder is directories
    if (m_folderPath.isEmpty()) {
      m_filesInfo[i].setFlags(FileInfo::DIRECTORY);
      m_filesInfo[i].setSize(0);
      m_filesInfo[i].setLastModified(0);
    }
  }

  delete[] fileNameList;

  return true;
}
