#pragma once
#include <vector>
#include "IpAccessRule.h"
#include "utils/Exception.h"
#include "io_lib/DataInputStream.h"
#include "io_lib/DataOutputStream.h"

class IpAccessControl: public std::vector<IpAccessRule*>
{
public:
  void serialize(DataOutputStream* output);
  void deserialize(DataInputStream* input);

};

