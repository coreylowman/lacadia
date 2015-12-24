#ifndef VEC3_H
#define VEC3_H

typedef union {
    float data[3];
    struct {
        float x, y, z;
    };
} Vec3;

void vec3_cross(Vec3 *out, Vec3 vec1, Vec3 vec2);
float vec3_dot(Vec3 vec1, Vec3 vec2);
void vec3_normalize(Vec3 *arr);

#endif
