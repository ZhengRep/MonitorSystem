#pragma once
#include "region/Region.h"
#include "region/Point.h"

class UpdateContainer
{
public:
  UpdateContainer();
  ~UpdateContainer();

  UpdateContainer(const UpdateContainer& updateContainer) { *this = updateContainer; }
  UpdateContainer& operator=(const UpdateContainer& src);

  Region copiedRegion;
  Region changedRegion;
  Region videoRegion;
  bool screenSizeChanged;
  bool cursorPosChanged;
  bool cursorShapeChanged;
  Point copySrc;
  Point cursorPos;

  void clear();
  bool isEmpty() const;
};

