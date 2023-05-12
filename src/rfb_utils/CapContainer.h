#pragma once
#include <vector>
#include "utils/inttypes.h"
#include "io_lib/DataOutputStream.h"

struct Cap { //capability
  UINT32 code;
  char vendorSignature[4];
  char nameSignature[8];
};

typedef std::vector<Cap> CapVector;
typedef std::vector<Cap>::const_iterator CapVectorConstIter;

class CapContainer
{
public:
  CapContainer();
  virtual ~CapContainer();

  void addCap(UINT32 code, const char* vendorSignature, const char* nameSignature);

  unsigned int getCapCount() const;
  void sendCaps(DataOutputStream* output) const;

  bool includes(UINT32 code) const;

private:
  CapVector m_caps;
};

