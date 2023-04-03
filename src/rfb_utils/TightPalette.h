#pragma once
#include <string>
#include "Utils/inttypes.h"

struct TightColorList {
  TightColorList* next;
  int idx;
  UINT32 rgb;
};

struct TightPaletteEntry {
  TightColorList* listNode;
  int numPixels;
};

class TightPalette
{
protected:
  inline static int hashFunc(UINT32 rgb) {
    return (rgb ^ (rgb >> 13)) & 0xFF;
  }

public:
  TightPalette(int maxColors = 254);
  void reset();
  void setMaxColors(int maxColors);
  int insert(UINT32 rgb, int numPixels);
  inline int getNumColors() const {
    return m_numColors;
  }

  inline UINT32 getEntry(int i) const {
    return (i < m_numColors) ? m_entry[i].listNode->rgb : (UINT32)- 1;
  }

  inline int getCount(int i) const {
    return (i < m_numColors) ? m_entry[i].numPixels : 0;
  }

  inline UINT8 getIndex(UINT32 rgb) const {
    TightColorList* pnode = m_hash[hashFunc(rgb)];
    while (pnode != NULL) {
      if (pnode->rgb == rgb) {
        return (UINT8)pnode->idx;
      }
      pnode = pnode->next;
    }
    return 0xFF;
  }

protected:
  int m_maxColors;
  int m_numColors;

  TightPaletteEntry m_entry[256];
  TightColorList* m_hash[256];
  TightColorList* m_list[256];
};

