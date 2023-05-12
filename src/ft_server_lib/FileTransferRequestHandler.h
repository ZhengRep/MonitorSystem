#pragma once
#include "utils/inttypes.h"
#include "network/RfbInputGate.h"
#include "network/RfbOutputGate.h"
#include "ft_common/FileInfo.h"
#include "file_lib/WinFileChannel.h"
#include "utils/Deflater.h"
#include "utils/Inflater.h"
#include "desktop/Desktop.h"
#include "rfb_utils/RfbCodeRegistrator.h"
#include "rfb_utils/RfbDispatcherListener.h"
#include "FileTransferSecurity.h"
#include "log_writer/LogWriter.h"

/*
  Handler of file transfer plugin client to server messages.
  Processes client requests and sends replies.
*/
class FileTransferRequestHandler: public RfbDispatcherListener
{
public:
  FileTransferRequestHandler(RfbCodeRegistrator* registrator, RfbOutputGate* output,
    Desktop* desktop, LogWriter* log, bool enabled = true);
  virtual ~FileTransferRequestHandler();
  virtual void onRequest(UINT32 reqCode, RfbInputGate* backGate);

protected:
  bool isFileTransferEnabled();

  // Common request handlers.
  void compressionSupportRequested();
  void fileListRequested();
  void mkDirRequested();
  void rmFileRequested();
  void mvFileRequested();
  void dirSizeRequested();
  void md5Requested();

 // Upload requests handlers.
  void uploadStartRequested();
  void uploadDataRequested();
  void uploadEndRequested();

  // Download requests handlers.
  void downloadStartRequested();
  void downloadDataRequested();

  // Method sends "Last request failed" message with error description.
  void lastRequestFailed(StringStorage* storage);
  void lastRequestFailed(const TCHAR* description);

  // Helper methods
  bool getDirectorySize(const TCHAR* pathname, UINT64* dirSize);

  void checkAccess() throw(SystemException);

protected:
  RfbInputGate* m_input;
  RfbOutputGate* m_output;

  //Download operation
  File* m_downloadFile;
  WinFileChannel* m_fileInputStream;

  //Upload operation
  File* m_uploadFile;
  WinFileChannel* m_fileOutputStream;

  Deflater m_deflater;
  Inflater m_inflater;

  FileTransferSecurity* m_security;
  bool m_enabled;
  LogWriter* m_log;
};

