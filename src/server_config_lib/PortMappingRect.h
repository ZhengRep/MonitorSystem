#pragma once
#include "utils/CommonHeader.h"
#include "utils/StringStorage.h"
#include "region/Rect.h"

class PortMappingRect: public Rect
{
public:
  PortMappingRect(int left, int top, int right, int bottom);
  PortMappingRect();
  virtual ~PortMappingRect();

  // Converts this rect to string value.
  void toString(StringStorage* string) const;

  // Returns true if string can be parsed to rect object,
  // false otherwise.
  static bool tryParse(const TCHAR* string);

  // Parsed string and sets parsed values to output rect.
  // Returns true if string is valid, false otherwise.
  // Remark: "out" parameter can be null.
  static bool parse(const TCHAR* string, PortMappingRect* out);
};

