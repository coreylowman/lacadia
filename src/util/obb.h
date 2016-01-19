#ifndef OBB_H
#define OBB_H

#include "vec3.h"

typedef struct {
    Vec3 center;
    Vec3 radius;
    Vec3 axis[3];
} Obb;

int obb_intersects(Obb a, Obb b);

#endif
