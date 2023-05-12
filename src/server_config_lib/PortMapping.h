#pragma once
#include "utils/StringStorage.h"
#include "PortMappingRect.h"

class PortMapping
{
public:
  PortMapping();
  PortMapping(int nport, PortMappingRect nrect);
  PortMapping(const PortMapping& other);
  virtual ~PortMapping();

  PortMapping& operator=(const PortMapping& other);
  bool isEqualTo(const PortMapping* other) const;

  void setPort(int nport);
  void setRect(PortMappingRect nrect);

  int getPort() const;
  PortMappingRect getRect() const;

  void toString(StringStorage* string) const;

public:
  static bool parse(const TCHAR* str, PortMapping* mapping);

protected:
  int m_port;

  PortMappingRect m_rect;
};

