#include "EncoderStore.h"
#include "RreEncoder.h"
#include "HextileEncoder.h"
#include "ZrleEncoder.h"
#include "TightEncoder.h"

EncoderStore::EncoderStore(PixelConverter* pixelConverter, DataOutputStream* output)
  :m_encoder(0),
  m_jpegEncoder(0),
  m_pixelConverter(pixelConverter),
  m_output(output)
{

}

EncoderStore::~EncoderStore()
{
  if (m_jpegEncoder != 0) {
    delete m_jpegEncoder;
  }
  // Remove all allocated encoders referenced in m_map.
  std::map<int, Encoder*>::iterator it;
  for (it = m_map.begin(); it != m_map.end(); it++) {
    delete it->second;
  }
}

Encoder* EncoderStore::getEncoder() const
{
  return m_encoder;
}

JpegEncoder* EncoderStore::getJpegEncoder() const
{
  return m_jpegEncoder;
}

void EncoderStore::selectEncoder(int encType)
{
  m_encoder = validateEncoder(encType);
}

void EncoderStore::validateJpegEncoder()
{
  if (m_jpegEncoder == 0) {
    TightEncoder* tight = (TightEncoder*)validateEncoder(EncodingDefs::TIGHT);
    m_jpegEncoder = new JpegEncoder(tight);
  }
}

Encoder* EncoderStore::validateEncoder(int encType)
{
  if (!encodingSupported(encType)) {
    encType = EncodingDefs::RAW;
  }
  std::map<int, Encoder*>::iterator it = m_map.find(encType);
  if (it != m_map.end()) {
    return it->second;
  }
  Encoder* newEncoder = allocateEncoder(encType);
  try {
    m_map[encType] = newEncoder;
  } catch (...) {
    delete newEncoder;
    throw;
  }
  return newEncoder;
}

bool EncoderStore::encodingSupported(int encType)
{
  return (encType == EncodingDefs::RAW ||
    encType == EncodingDefs::RRE ||
    encType == EncodingDefs::HEXTILE ||
    encType == EncodingDefs::ZRLE ||
    encType == EncodingDefs::TIGHT);
}

Encoder* EncoderStore::allocateEncoder(int encType) const
{
  switch (encType) {
  case EncodingDefs::TIGHT:
    return new TightEncoder(m_pixelConverter, m_output);
  case EncodingDefs::ZRLE:
    return new ZrleEncoder(m_pixelConverter, m_output);
  case EncodingDefs::HEXTILE:
    return new HextileEncoder(m_pixelConverter, m_output);
  case EncodingDefs::RRE:
    return new RreEncoder(m_pixelConverter, m_output);
  case EncodingDefs::RAW:
    return new Encoder(m_pixelConverter, m_output);
  default:
    throw Exception(_T("Cannot create encoder of the specified type"));
  }
}
