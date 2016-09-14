#ifndef RECT_H
#define RECT_H

typedef struct {
  float x1, y1;
  float x2, y2;
  float x3, y3;
  float x4, y4;
} UIRect;

typedef struct {
  float x, y;
  float width, height;
} Rect2;

typedef struct {
  float x, y, z;
  float width, height, length;
} Rect3;

UIRect ui_rect_from_rect2(Rect2 rect);

#endif
