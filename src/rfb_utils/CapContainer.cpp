#include "CapContainer.h"

CapContainer::CapContainer()
{
}

CapContainer::~CapContainer()
{
}

void CapContainer::addCap(UINT32 code, const char* vendorSignature, const char* nameSignature)
{
  _ASSERT(strlen(vendorSignature) == 4);
  _ASSERT(strlen(nameSignature) == 8);
  Cap cap;
  cap.code = code;
  memcpy(cap.vendorSignature, vendorSignature, 4);
  memcpy(cap.nameSignature, nameSignature, 8);
  m_caps.push_back(cap);
}

unsigned int CapContainer::getCapCount() const
{
  unsigned int capsSize = (unsigned int)m_caps.size();
  _ASSERT(capsSize == m_caps.size());
  return capsSize;
}

void CapContainer::sendCaps(DataOutputStream* output) const
{
  for (CapVectorConstIter iter = m_caps.begin(); iter < m_caps.end(); iter++) {
    output->writeUInt32((*iter).code);
    output->writeFully(&(*iter).vendorSignature, 4);
    output->writeFully(&(*iter).nameSignature, 8);
  }
}

bool CapContainer::includes(UINT32 code) const
{
  bool result = false;
  for (CapVectorConstIter iter = m_caps.begin(); iter < m_caps.end(); iter++) {
    if ((*iter).code == code) {
      result = true;
      break;
    }
  }
  return result;
}
