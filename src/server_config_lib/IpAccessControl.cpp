#include "IpAccessControl.h"

void IpAccessControl::serialize(DataOutputStream* output)
{
  _ASSERT((unsigned int)size() == size());
  unsigned int count = (unsigned int)size();
  output->writeUInt32(count);

  StringStorage string;

  for (size_t i = 0; i < count; i++) {
    IpAccessRule* rule = at(i);
    rule->toString(&string);
    output->writeUTF8(string.getString());
  }
}

void IpAccessControl::deserialize(DataInputStream* input)
{
  for (iterator i = begin(); i != end(); ++i) {
    delete* i;
  }

  size_t count = input->readUInt32();
  resize(count);

  StringStorage string;

  for (iterator i = begin(); i != end(); ++i) {
    input->readUTF8(&string);

    // Here is would be good to use auto_ptr, but
    // auto_ptr is not compatible with vector.
    IpAccessRule* rule = new IpAccessRule();
    try {
      if (!IpAccessRule::parse(string.getString(), rule)) {
        throw Exception(_T("Parsing of ip access rule is failed."));
      }
    } catch (...) {
      delete rule;
      rule = 0;
      throw;
    }

    *i = rule;
  }
}
