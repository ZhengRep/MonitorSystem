#pragma once

#include "Rect.h"

class Dimension
{
public:
    Dimension(void) : width(0), height(0) {}
    Dimension(const int w, const int h) { setDim(w, h); }
    Dimension(const Rect* r) { width = r->getWidth(); height = r->getHeight(); }
    Dimension(const Dimension& other) : width(other.width), height(other.height) {}

    const Dimension& operator=(const Dimension& other) {
        width = other.width;
        height = other.height;
        return *this;
    }
    bool operator==(const Dimension& other) { return isEqualTo(&other); }
    bool operator!=(const Dimension& other) { return !isEqualTo(&other); }

    virtual ~Dimension(void) {}

    Rect getRect() const
    {
        Rect r(width, height);
        return r;
    }

    Dimension getTransposition() const
    {
        Dimension r(height, width);
        return r;
    }

    // Exchanges width and height
    void transpose()
    {
        int temp = width;
        width = height;
        height = temp;
    }

    inline void setDim(const int w, const int h) { width = w; height = h; }
    inline void setDim(const Rect* rect) {
        width = rect->getWidth();
        height = rect->getHeight();
    }

    inline bool cmpDim(const Dimension* dim) const {
        return dim->width == width &&
            dim->height == height;
    }

    inline bool isEmpty() const { return width <= 0 || height <= 0; }
    inline bool isEqualTo(const Dimension* other) const
    {
        return (width == other->width) && (height == other->height);
    }
    inline int area() const { return isEmpty() ? 0 : width * height; }

    inline void clear() { width = height = 0; }

    inline Dimension intersection(const Dimension* other) const
    {
        return Dimension(width < other->width ? width : other->width,
            height < other->height ? height : other->height);
    }

    int width;
    int height;
};

