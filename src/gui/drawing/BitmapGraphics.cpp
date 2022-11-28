#include "BitmapGraphics.h"

BitmapGraphics::BitmapGraphics(DeviceContext* compatibleDC, int width, int height)
    :Graphics(compatibleDC), m_isPainting(false)
{
    m_bitmap = new Bitmap(compatibleDC->m_dc, width, height);
    m_dc = new DeviceContext(compatibleDC);
}

BitmapGraphics::~BitmapGraphics()
{
    delete m_bitmap;
    delete m_dc;
}

Bitmap* BitmapGraphics::getBitmap()
{
    return m_bitmap;
}

void BitmapGraphics::beginPaint()
{
    _ASSERT(!m_isPainting);
    m_isPainting = true;
    m_oldBitmap = m_dc->selectObject(m_bitmap->m_bitmap);
}

void BitmapGraphics::endPaint()
{
    _ASSERT(m_isPainting);
    m_isPainting = false;
    m_dc->selectObject(m_oldBitmap);
}
