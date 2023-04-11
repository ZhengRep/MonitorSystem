#include "PortMappingContainer.h"

PortMappingContainer::PortMappingContainer()
{
}

PortMappingContainer::PortMappingContainer(const PortMappingContainer& other)
  :m_vector(other.m_vector)
{
}

PortMappingContainer::~PortMappingContainer()
{
}

PortMappingContainer& PortMappingContainer::operator=(const PortMappingContainer& other)
{
  m_vector = other.m_vector;
  return *this;
}

void PortMappingContainer::pushBack(PortMapping element)
{
  m_vector.push_back(element);
}

size_t PortMappingContainer::find(PortMapping searchElement) const
{
  for (size_t i = 0; i < count(); i++) {
    PortMapping each = m_vector.at(i);
    PortMappingRect rect = each.getRect();
    if (each.getPort() == searchElement.getPort() &&
      each.getRect().isEqualTo(&rect)) {
      return i;
    }
  }
  return (size_t)-1;
}

size_t PortMappingContainer::findByPort(int port) const
{
  for (size_t i = 0; i < count(); i++) {
    if (m_vector.at(i).getPort() == port) {
      return i;
    }
  }
  return (size_t)-1;
}

void PortMappingContainer::remove(size_t index)
{
  size_t i = 0;
  for (std::vector<PortMapping>::iterator it = m_vector.begin();
    it != m_vector.end(); it++) {
    if (i == index) {
      m_vector.erase(it);
      break;
    }
    i++;
  }
}

void PortMappingContainer::remove(PortMapping removeMapping)
{
  size_t index = find(removeMapping);

  if (index != (size_t)-1) {
    remove(index);
  }
}

void PortMappingContainer::removeAll()
{
  m_vector.clear();
}

size_t PortMappingContainer::count() const
{
  return m_vector.size();
}

bool PortMappingContainer::eqauls(const PortMappingContainer* other) const
{
  if (count() != other->count()) {
    return false;
  }
  for (size_t i = 0; i < count(); i++) {
    if (!at(i)->isEqualTo(other->at(i))) {
      return false;
    }
  }
  return true;
}

void PortMappingContainer::serialize(DataOutputStream* output) const throw(Exception)
{
  _ASSERT((unsigned int)count() == count());
  output->writeUInt32((unsigned int)count());

  StringStorage string;

  for (size_t i = 0; i < count(); i++) {
    at(i)->toString(&string);

    output->writeUTF8(string.getString());
  }
}

void PortMappingContainer::deserialize(DataInputStream* input) throw(Exception)
{
  removeAll();

  size_t cnt = input->readUInt32();

  StringStorage string;

  PortMapping record;

  for (size_t i = 0; i < cnt; i++) {

    input->readUTF8(&string);

    if (!PortMapping::parse(string.getString(), &record)) {
      throw Exception(_T("Invalid port mapping string"));
    }

    pushBack(record);
  }
}

const PortMapping* PortMappingContainer::at(size_t index) const
{
  bool outOfRange = index >= count();

  return outOfRange ? 0 : &m_vector.at(index);
}

PortMapping* PortMappingContainer::at(size_t index)
{
  bool outOfRange = index >= count();

  return outOfRange ? 0 : &m_vector.at(index);
}
