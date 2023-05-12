#pragma once
#include "utils/inttypes.h"

/*
 * format:
 *  UINT32 messageId 32 bit message id.
 *  UINT8[] messageBody message body depends on messageId.
 * 
 * Structures used in file transfer proto:
 *
 *  struct StringUTF8 { string encoded in UTF8 encoding.
 *    UINT32 size;        // size of encoded string buffer in bytes;
 *    UINT8 buffer[size]; // buffer with encoded string data.
 * File transfer sends and recives file names in unix-like format (sample: "/C:/folder1/folder2".
 * 
 * "CompressedBlock:" has several mandatory fields:
 *   UINT8 compressionLevel compression level (0 - not compressed, [1..9] compressed).
 *   UINT32 compressedSize size of compressed data.
 *   UINT32 uncompressedSize size of uncompressed data.
 *   UINT8 compressedData[compressedSize] compressed usefull data.
*/
class FTMessage
{
public:
  const static char COMPRESSION_SUPPORT_REQUEST_SIG[];
  const static char COMPRESSION_SUPPORT_REPLY_SIG[];
  const static UINT32 COMPRESSION_SUPPORT_REQUEST = 0xFC000100;
  const static UINT32 COMPRESSION_SUPPORT_REPLY = 0xFC000101;

  const static char FILE_LIST_REQUEST_SIG[];
  const static char FILE_LIST_REPLY_SIG[];
  const static UINT32 FILE_LIST_REQUEST = 0xFC000102;
  const static UINT32 FILE_LIST_REPLY = 0xFC000103;

  const static char MD5_REQUEST_SIG[];
  const static char MD5_REPLY_SIG[];
  const static UINT32 MD5_REQUEST = 0xFC000104;
  const static UINT32 MD5_REPLY = 0xFC000105;

  const static char UPLOAD_START_REQUEST_SIG[];
  const static char UPLOAD_START_REPLY_SIG[];
  const static UINT32 UPLOAD_START_REQUEST = 0xFC000106;
  const static UINT32 UPLOAD_START_REPLY = 0xFC000107;

  const static char UPLOAD_DATA_REQUEST_SIG[];
  const static char UPLOAD_DATA_REPLY_SIG[];
  const static UINT32 UPLOAD_DATA_REQUEST = 0xFC000108;
  const static UINT32 UPLOAD_DATA_REPLY = 0xFC000109;

  const static char UPLOAD_END_REQUEST_SIG[];
  const static char UPLOAD_END_REPLY_SIG[];
  const static UINT32 UPLOAD_END_REQUEST = 0xFC00010A;
  const static UINT32 UPLOAD_END_REPLY = 0xFC00010B;

  const static char DOWNLOAD_START_REQUEST_SIG[];
  const static char DOWNLOAD_START_REPLY_SIG[];
  const static UINT32 DOWNLOAD_START_REQUEST = 0xFC00010C;
  const static UINT32 DOWNLOAD_START_REPLY = 0xFC00010D;

  const static char DOWNLOAD_DATA_REQUEST_SIG[];
  const static char DOWNLOAD_DATA_REPLY_SIG[];
  const static UINT32 DOWNLOAD_DATA_REQUEST = 0xFC00010E;
  const static UINT32 DOWNLOAD_DATA_REPLY = 0xFC00010F;

  const static char DOWNLOAD_END_REPLY_SIG[];
  const static UINT32 DOWNLOAD_END_REPLY = 0xFC000110;

  const static char MKDIR_REQUEST_SIG[];
  const static char MKDIR_REPLY_SIG[];
  const static UINT32 MKDIR_REQUEST = 0xFC000111;
  const static UINT32 MKDIR_REPLY = 0xFC000112;

  const static char REMOVE_REQUEST_SIG[];
  const static char REMOVE_REPLY_SIG[];
  const static UINT32 REMOVE_REQUEST = 0xFC000113;
  const static UINT32 REMOVE_REPLY = 0xFC000114;

  const static char RENAME_REQUEST_SIG[];
  const static char RENAME_REPLY_SIG[];
  const static UINT32 RENAME_REQUEST = 0xFC000115;
  const static UINT32 RENAME_REPLY = 0xFC000116;

  const static char DIRSIZE_REQUEST_SIG[];
  const static char DIRSIZE_REPLY_SIG[];
  const static UINT32 DIRSIZE_REQUEST = 0xFC000117;
  const static UINT32 DIRSIZE_REPLY = 0xFC000118;

  const static UINT32 LAST_REQUEST_FAILED_REPLY = 0xFC000119;
  const static char LAST_REQUEST_FAILED_REPLY_SIG[];
};

