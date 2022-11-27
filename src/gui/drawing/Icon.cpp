#include "Icon.h"

Icon::Icon()
    :m_hasOwnIcon(true), m_icon(NULL)
{
}

Icon::Icon(HICON icon)
    :m_hasOwnIcon(true), m_icon(icon)
{
}

Icon::Icon(Bitmap* bitmap)
    :m_hasOwnIcon(true), m_icon(NULL)
{
    Bitmap mask(bitmap->getWidth(), bitmap->getHeight());
    fromBitmap(bitmap, &mask);
}

Icon::Icon(Bitmap* bitmap, Bitmap* mask)
    :m_hasOwnIcon(true), m_icon(NULL)
{
    fromBitmap(bitmap, mask);
}

Icon::Icon(DWORD icon)
    :m_hasOwnIcon(false)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    m_icon = LoadIcon(hInstance, MAKEINTRESOURCE(icon));
}

Icon::~Icon()
{
    if (m_hasOwnIcon) {
        DestroyIcon(m_icon);
    }
}

HICON Icon::getHICON()
{
    return m_icon;
}

void Icon::fromBitmap(Bitmap* bitmap, Bitmap* mask)
{
    ICONINFO ii;
    memset(&ii, 0, sizeof(ICONINFO));
    ii.hbmColor = (bitmap != 0) ? bitmap->m_bitmap : 0;
    ii.hbmMask = (mask != 0) ? mask->m_bitmap : 0;
    m_icon = CreateIconIndirect(&ii);
}
