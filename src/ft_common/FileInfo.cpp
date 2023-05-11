#include "FileInfo.h"

FileInfo::FileInfo()
  : m_sizeInBytes(0), m_lastModified(0), m_flags(0)
{
}

FileInfo::FileInfo(UINT64 size, UINT64 modTime, UINT16 flags, const TCHAR* fileName)
  : m_sizeInBytes(size), m_lastModified(modTime), m_flags(flags)
{
  m_fileName.setString(fileName);
}

FileInfo::FileInfo(const File* file)
{
m_sizeInBytes = file->length();
m_lastModified = file->lastModified();
m_flags = 0;
if (file->isDirectory()) {
  m_flags |= FileInfo::DIRECTORY;
  m_sizeInBytes = 0;
  m_lastModified = 0;
}
file->getName(&m_fileName);
}

bool FileInfo::isExecutable() const
{
  return (m_flags & FileInfo::EXECUTABLE) ? true : false;
}

void FileInfo::setLastModified(UINT64 time)
{
  m_lastModified = time;
}

void FileInfo::setSize(UINT64 size)
{
  m_sizeInBytes = size;
}

void FileInfo::setFlags(UINT16 flags)
{
  m_flags = flags;
}

void FileInfo::setFileName(const TCHAR* fileName)
{
  m_fileName.setString(fileName);
}

UINT64 FileInfo::lastModified() const
{
  return m_lastModified;
}

UINT64 FileInfo::getSize() const
{
  return m_sizeInBytes;
}

UINT16 FileInfo::getFlags() const
{
  return m_flags;
}

const TCHAR* FileInfo::getFileName() const
{
  return m_fileName.getString();
}

