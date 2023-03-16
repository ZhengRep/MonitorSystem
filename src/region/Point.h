#pragma once

struct Point {
  Point() : x(0), y(0) {};
  Point(int x_, int y_) :x(x_), y(y_) {};
  
  inline void clear() { 
    x = 0; y = 0; 
  }
  inline void setPoint(int x_, int y_) { 
    x = x_; 
    y = y_; 
  }
  inline void move(int deltaX, int deltaY) { 
    x += deltaX; 
    y += deltaY; 
  }
  bool isEqualTo(const Point* other) const {
    return x == other->x && y == other->y;
  }

  int x, y;
};
