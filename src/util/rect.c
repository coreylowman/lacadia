#include <math.h>
#include "rect.h"

int rect_intersects(Rect r1, Rect r2){
    return abs(r1.x - r2.x) * 2 < (r1.width + r2.width)
        && abs(r1.y - r2.y) * 2 < (r1.height + r2.height)
        && abs(r1.z - r2.z) * 2 < (r1.length + r2.length);
}
