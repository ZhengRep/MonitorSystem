#pragma once
#include "PortMapping.h"
#include "utils/Exception.h"
#include "io_lib/DataInputStream.h"
#include "io_lib/DataOutputStream.h"
#include <vector>

class PortMappingContainer
{
public:
  PortMappingContainer();
  PortMappingContainer(const PortMappingContainer& other);
  virtual ~PortMappingContainer();
  PortMappingContainer& operator=(const PortMappingContainer& other);

  void pushBack(PortMapping element);
  size_t find(PortMapping searchElement) const;
  size_t findByPort(int port) const;
  void remove(size_t index);
  void remove(PortMapping removeMapping);
  void removeAll();
  size_t count() const;
  bool eqauls(const PortMappingContainer* other) const;

  //Serializes port all mappings in container as byte stream
  void serialize(DataOutputStream* output) const throw(Exception);

  void deserialize(DataInputStream* input) throw(Exception);
  const PortMapping* at(size_t index) const;
  PortMapping* at(size_t index);

private:
  std::vector<PortMapping> m_vector;
};

