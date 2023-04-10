#pragma once
#include <stdio.h>
#include "Utils/CommonHeader.h"
#include "rfb/PixelFormat.h""
#include "libjpeg/jpeglib.h"

class JpegCompressor
{
public:
  virtual ~JpegCompressor() {}

  // Set JPEG quality level (0..100)
  virtual void setQuality(int level) = 0;
  virtual void resetQuality() = 0;
  virtual void compress(const void* buf, const PixelFormat* fmt, int w, int h, int stride) = 0;
  virtual size_t getOutputLength() = 0;
  virtual const char* getOutputData() = 0;
};

class StandardJpegCompressor : public JpegCompressor
{
public:
  StandardJpegCompressor();
  virtual ~StandardJpegCompressor();

  virtual void setQuality(int level);
  virtual void resetQuality();

  virtual void compress(const void* buf, const PixelFormat* fmt,
    int w, int h, int stride);

  virtual size_t getOutputLength();
  virtual const char* getOutputData();

public:
  void initDestination();
  bool emptyOutputBuffer();
  void termDestination();

  static const int ALLOC_CHUNK_SIZE;
  static const int DEFAULT_JPEG_QUALITY;

  int m_quality;
  int m_newQuality;

  unsigned char* m_outputBuffer;
  size_t m_numBytesAllocated;
  size_t m_numBytesReady;
  void convertRow(JSAMPLE* dst, const void* src, const PixelFormat* fmt, int numPixels);
  void convertRow24(JSAMPLE* dst, const void* src, const PixelFormat* fmt, int numPixels);

private:
  METHODDEF(StringStorage) getMessage(j_common_ptr cinfo);
  METHODDEF(void) errorExit(j_common_ptr cinfo);
  METHODDEF(void) outputMessage(j_common_ptr cinfo);

  typedef struct _TC_JPEG_COMPRESSOR {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
  } TC_JPEG_COMPRESSOR;

  TC_JPEG_COMPRESSOR m_jpeg;
};