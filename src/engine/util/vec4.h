#ifndef VEC4_H
#define VEC4_H

typedef union {
  float data[4];
  struct {
    float x, y, z, w;
  };
} Vec4;

static Vec4 VEC4_UNIT_X = {.data = {1, 0, 0, 0}};
static Vec4 VEC4_UNIT_Y = {.data = {0, 1, 0, 0}};
static Vec4 VEC4_UNIT_Z = {.data = {0, 0, 1, 0}};
static Vec4 VEC4_UNIT_NX = {.data = {-1, 0, 0, 0}};
static Vec4 VEC4_UNIT_NY = {.data = {0, -1, 0, 0}};
static Vec4 VEC4_UNIT_NZ = {.data = {0, 0, -1, 0}};
static Vec4 VEC4_ZERO = {.data = {0, 0, 0, 0}};

#endif
