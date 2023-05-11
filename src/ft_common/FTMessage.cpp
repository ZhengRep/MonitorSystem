#include "FTMessage.h"

const char FTMessage::COMPRESSION_SUPPORT_REQUEST_SIG[] = "FTCCSRST";
const char FTMessage::COMPRESSION_SUPPORT_REPLY_SIG[] = "FTSCSRLY";
const char FTMessage::FILE_LIST_REQUEST_SIG[] = "FTCFLRST";
const char FTMessage::FILE_LIST_REPLY_SIG[] = "FTSFLRLY";
const char FTMessage::MD5_REQUEST_SIG[] = "FTCM5RST";
const char FTMessage::MD5_REPLY_SIG[] = "FTSM5RLY";
const char FTMessage::UPLOAD_START_REQUEST_SIG[] = "FTCFURST";
const char FTMessage::UPLOAD_START_REPLY_SIG[] = "FTSFURLY";
const char FTMessage::UPLOAD_DATA_REQUEST_SIG[] = "FTCUDRST";
const char FTMessage::UPLOAD_DATA_REPLY_SIG[] = "FTSUDRLY";
const char FTMessage::UPLOAD_END_REQUEST_SIG[] = "FTCUERST";
const char FTMessage::UPLOAD_END_REPLY_SIG[] = "FTSUERLY";
const char FTMessage::DOWNLOAD_START_REQUEST_SIG[] = "FTCFDRST";
const char FTMessage::DOWNLOAD_START_REPLY_SIG[] = "FTSFDRLY";
const char FTMessage::DOWNLOAD_DATA_REQUEST_SIG[] = "FTCDDRST";
const char FTMessage::DOWNLOAD_DATA_REPLY_SIG[] = "FTSDDRLY";
const char FTMessage::DOWNLOAD_END_REPLY_SIG[] = "FTSDERLY";
const char FTMessage::MKDIR_REQUEST_SIG[] = "FTCMDRST";
const char FTMessage::MKDIR_REPLY_SIG[] = "FTSMDRLY";
const char FTMessage::REMOVE_REQUEST_SIG[] = "FTCFRRST";
const char FTMessage::REMOVE_REPLY_SIG[] = "FTSFTRLY";
const char FTMessage::RENAME_REQUEST_SIG[] = "FTCFMRST";
const char FTMessage::RENAME_REPLY_SIG[] = "FTSFMRLY";
const char FTMessage::DIRSIZE_REQUEST_SIG[] = "FTCDSRST";
const char FTMessage::DIRSIZE_REPLY_SIG[] = "FTSDSRLY";
const char FTMessage::LAST_REQUEST_FAILED_REPLY_SIG[] = "FTLRFRLY";
