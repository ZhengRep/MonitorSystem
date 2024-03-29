#pragma once

#include "TightPalette.h"
#include "utils/inttypes.h"
#include <crtdbg.h>

template<class PIXEL_T> class HextileTile
{
public:
  HextileTile();
  ~HextileTile();

  void newTile(const PIXEL_T* src, int w, int h);

  int getFlags() const { return m_flags; }

  size_t getSize() const { return m_size; }


  int getBackground() const { return m_background; }

  int getForeground() const { return m_foreground; }

  void encode(UINT8* dst) const;

protected:


  void analyze();

  const PIXEL_T* m_tile;
  int m_width;
  int m_height;

  size_t m_size;
  int m_flags;
  PIXEL_T m_background;
  PIXEL_T m_foreground;

  int m_numSubrects;
  UINT8 m_coords[256 * 2];
  PIXEL_T m_colors[256];

private:

  bool m_processed[16][16];
  TightPalette m_pal;
};


const int hextileRaw = 1;
const int hextileBgSpecified = 2;
const int hextileFgSpecified = 4;
const int hextileAnySubrects = 8;
const int hextileSubrectsColoured = 16;

template<class PIXEL_T> HextileTile<PIXEL_T>::HextileTile()
  : m_tile(NULL), m_width(0), m_height(0),
  m_size(0), m_flags(0), m_background(0), m_foreground(0),
  m_numSubrects(0), m_pal(48 + 2 * sizeof(PIXEL_T))
{
}

template<class PIXEL_T> HextileTile<PIXEL_T>::~HextileTile()
{
}

template<class PIXEL_T> void HextileTile<PIXEL_T>::newTile(const PIXEL_T* src,
  int w, int h)
{
  m_tile = src;
  m_width = w;
  m_height = h;

  analyze();
}

template<class PIXEL_T> void HextileTile<PIXEL_T>::analyze()
{
  _ASSERT(m_tile && m_width && m_height);

  const PIXEL_T* ptr = m_tile;
  const PIXEL_T* end = &m_tile[m_width * m_height];
  PIXEL_T color = *ptr++;
  while (ptr != end && *ptr == color)
    ptr++;

  // Handle solid tile
  if (ptr == end) {
    m_background = m_tile[0];
    m_flags = 0;
    m_size = 0;
    return;
  }

  // Compute number of complete rows of the same color, at the top
  int y = (int)(ptr - m_tile) / m_width;

  PIXEL_T* colorsPtr = m_colors;
  UINT8* coordsPtr = m_coords;
  m_pal.reset();
  m_numSubrects = 0;

  // Have we found the first subrect already?
  if (y > 0) {
    *colorsPtr++ = color;
    *coordsPtr++ = 0;
    *coordsPtr++ = (UINT8)(((m_width - 1) << 4) | ((y - 1) & 0x0F));
    m_pal.insert(color, 1);
    m_numSubrects++;
  }

  memset(m_processed, 0, 16 * 16 * sizeof(bool));

  int x, sx, sy, sw, sh, max_x;

  for (; y < m_height; y++) {
    for (x = 0; x < m_width; x++) {
      // Skip pixels that were processed earlier
      if (m_processed[y][x]) {
        continue;
      }
      // Determine dimensions of the horizontal subrect
      color = m_tile[y * m_width + x];
      for (sx = x + 1; sx < m_width; sx++) {
        if (m_tile[y * m_width + sx] != color)
          break;
      }
      sw = sx - x;
      max_x = sx;
      for (sy = y + 1; sy < m_height; sy++) {
        for (sx = x; sx < max_x; sx++) {
          if (m_tile[sy * m_width + sx] != color)
            goto done;
        }
      }
    done:
      sh = sy - y;

      // Save properties of this subrect
      *colorsPtr++ = color;
      *coordsPtr++ = (UINT8)((x << 4) | (y & 0x0F));
      *coordsPtr++ = (UINT8)(((sw - 1) << 4) | ((sh - 1) & 0x0F));

      if (m_pal.insert(color, 1) == 0) {
        // Handle palette overflow
        m_flags = hextileRaw;
        m_size = 0;
        return;
      }

      m_numSubrects++;

      // Mark pixels of this subrect as processed, below this row
      for (sy = y + 1; sy < y + sh; sy++) {
        for (sx = x; sx < x + sw; sx++)
          m_processed[sy][sx] = true;
      }

      // Skip processed pixels of this row
      x += (sw - 1);
    }
  }

  // Save number of colors in this tile (should be no less than 2)
  int numColors = m_pal.getNumColors();
  _ASSERT(numColors >= 2);

  m_background = (PIXEL_T)m_pal.getEntry(0);
  m_flags = hextileAnySubrects;
  int numSubrects = m_numSubrects - m_pal.getCount(0);

  if (numColors == 2) {
    // Monochrome tile
    m_foreground = (PIXEL_T)m_pal.getEntry(1);
    m_size = 1 + 2 * numSubrects;
  } else {
    // Colored tile
    m_flags |= hextileSubrectsColoured;
    m_size = 1 + (2 + sizeof(PIXEL_T)) * numSubrects;
  }
}

template<class PIXEL_T> void HextileTile<PIXEL_T>::encode(UINT8* dst) const
{
  _ASSERT(m_numSubrects && (m_flags & hextileAnySubrects));

  // Zero subrects counter
  UINT8* numSubrectsPtr = dst;
  *dst++ = 0;

  for (int i = 0; i < m_numSubrects; i++) {
    if (m_colors[i] == m_background) {
      continue;
    }
    if (m_flags & hextileSubrectsColoured) {
      *(PIXEL_T*)dst = m_colors[i];
      dst += sizeof(PIXEL_T);
    }
    *dst++ = m_coords[i * 2];
    *dst++ = m_coords[i * 2 + 1];

    (*numSubrectsPtr)++;
  }

  _ASSERT(dst - numSubrectsPtr == m_size);
}
