#pragma once
#include<map>
#include "Encoder.h"
#include "JpegEncoder.h"

class EncoderStore
{
 public:
  EncoderStore(PixelConverter *pixelConverter, DataOutputStream *output);
  ~EncoderStore();
  Encoder *getEncoder() const;
  JpegEncoder* getJpegEncoder() const;

  void selectEncoder(int encType);
  void validateJpegEncoder();

protected:
  Encoder* validateEncoder(int encType);
  static bool encodingSupported(int encType);
  Encoder* allocateEncoder(int encType) const;

protected:
  std::map<int, Encoder*> m_map;
  Encoder* m_encoder;
  JpegEncoder* m_jpegEncoder;
  PixelConverter* m_pixelConverter;
  DataOutputStream* m_output;

private:
  EncoderStore(const EncoderStore& other);
  EncoderStore& operator=(const EncoderStore& other);
};

