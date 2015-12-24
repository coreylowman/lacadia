#ifndef RECT_H
#define RECT_H

typedef struct {
    float x, y, z;
    float width, height, length;
} Rect;

int rect_intersects(Rect r1, Rect r2);

#endif
