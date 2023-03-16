#pragma once
#include<vector>
#include<list>
#include "Rect.h"

extern "C" {
#include "x11region.h"
}

class Region
{
public:
  Region();
  Region(const Rect& rect);
  Region(const Region& src);
  virtual ~Region();
  void clear();
  void set(const Region* src);
  Region& operator=(const Region& src);
  void addRect(const Rect* rect);
  void translate(int dx, int dy);
  void add(const Region* other);
  void add(const Region& other);
  void substract(const Region* other);
  void intersect(const Region* other);
  void crop(const Rect* rect);
  bool isEmpty() const;
  bool isPointInside(int x, int y) const;
  bool equals(const Region* other) const;
  void getRectVector(std::vector<Rect>* dst) const;
  void getRectList(std::list<Rect>* dst) const;
  size_t getCount() const;
  Rect getBounds() const;

private:
  RegionRec m_reg;
};

