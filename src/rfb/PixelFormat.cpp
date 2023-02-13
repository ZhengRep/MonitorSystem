#include "PixelFormat.h"
#include <string.h>

PixelFormat::PixelFormat()
{
    memset(this, 0, sizeof(PixelFormat));
}

void PixelFormat::initBigEndianByNative()
{
    union {
        char test;
        int i;
    }testBigEndian;

    testBigEndian.i = 1;
    bigEndian = (testBigEndian.test == 0);
}

bool PixelFormat::isEqualTo(const PixelFormat* pf) const
{
    return bitsPerPixel == pf->bitsPerPixel &&
        colorDepth == pf->colorDepth &&
        redMax == pf->redMax &&
        greenMax == pf->greenMax &&
        blueMax == pf->blueMax &&
        redShift == pf->redShift &&
        greenShift == pf->greenShift &&
        blueShift == pf->blueShift &&
        bigEndian == pf->bigEndian;
}
