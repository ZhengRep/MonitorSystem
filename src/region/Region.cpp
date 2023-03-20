#include "Region.h"

Region::Region()
{
  miRegionInit(&m_reg, NullBox, 0);
}

Region::Region(const Rect& rect)
{
  if (!rect.isEmpty()) {
    BoxRec box;
    box.x1 = rect.left;
    box.x2 = rect.right;
    box.y1 = rect.top;
    box.y2 = rect.bottom;
    miRegionInit(&m_reg, &box, 0);
  } else {
    miRegionInit(&m_reg, NullBox, 0);
  }
}

Region::Region(const Region& src)
{
  miRegionInit(&m_reg, NullBox, 0);
  set(&src);
}

Region::~Region()
{
  miRegionUninit(&m_reg);
}

void Region::clear()
{
  miRegionEmpty(&m_reg);
}

void Region::set(const Region* src)
{
  miRegionCopy(&m_reg, (RegionPtr)&src->m_reg);
}

Region& Region::operator=(const Region& src)
{
    // TODO: insert return statement here
  set(&src);
  return *this;
}

void Region::addRect(const Rect* rect)
{
  if (!rect->isEmpty()) {
    Region temp(rect);
    add(&temp);
  }
}

void Region::translate(int dx, int dy)
{
  miTranslateRegion(&m_reg, dx, dy);
}

void Region::add(const Region* other)
{
  miUnion(&m_reg, &m_reg, (RegionPtr)&other->m_reg);
}

void Region::add(const Region& other)
{
  miUnion(&m_reg, &m_reg, (RegionPtr)&other.m_reg);
}

void Region::substract(const Region* other)
{
  miSubtract(&m_reg, &m_reg, (RegionPtr)&other->m_reg);
}

void Region::intersect(const Region* other)
{
  miIntersect(&m_reg, &m_reg, (RegionPtr)&other->m_reg);
}

void Region::crop(const Rect* rect)
{
  Region temp(rect);
  intersect(&temp);
}

bool Region::isEmpty() const
{
  return (miRegionNotEmpty((RegionPtr)&m_reg) == FALSE);
}

bool Region::isPointInside(int x, int y) const
{
  BoxRec stubBox; // Ignore returning rect.
  return !!miPointInRegion((RegionPtr)&m_reg, x, y, &stubBox);
}

bool Region::equals(const Region* other) const
{
  if (this->isEmpty() && other->isEmpty()) {
    return true;
  }

  return (miRegionsEqual((RegionPtr)&m_reg, (RegionPtr)&other->m_reg) == TRUE);
}

void Region::getRectVector(std::vector<Rect>* dst) const
{
  dst->clear();

  const BoxRec* boxPtr = REGION_RECTS(&m_reg);
  long numRects = REGION_NUM_RECTS(&m_reg);
  dst->reserve((size_t)numRects);
  for (long i = 0; i < numRects; i++) {
    Rect rect(boxPtr[i].x1, boxPtr[i].y1, boxPtr[i].x2, boxPtr[i].y2);
    dst->push_back(rect);
  }
}

void Region::getRectList(std::list<Rect>* dst) const
{
  dst->clear();

  const BoxRec* boxPtr = REGION_RECTS(&m_reg);
  long numRects = REGION_NUM_RECTS(&m_reg);
  for (long i = 0; i < numRects; i++) {
    Rect rect(boxPtr[i].x1, boxPtr[i].y1, boxPtr[i].x2, boxPtr[i].y2);
    dst->push_back(rect);
  }
}

size_t Region::getCount() const
{
  return REGION_NUM_RECTS(&m_reg);
}

Rect Region::getBounds() const
{
  const BoxRec* boxPtr = REGION_EXTENTS(&m_reg);
  return Rect(boxPtr->x1, boxPtr->y1, boxPtr->x2, boxPtr->y2);
}
