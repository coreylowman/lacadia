#ifndef VEC3_H
#define VEC3_H

typedef union {
    float data[3];
    struct {
        float x, y, z;
    };
} Vec3;

Vec3 vec3_from3f(float x, float y, float z);
Vec3 vec3_add(Vec3 v1, Vec3 v2);
Vec3 vec3_sub(Vec3 v1, Vec3 v2);
Vec3 vec3_cross(Vec3 vec1, Vec3 vec2);
float vec3_dot(Vec3 vec1, Vec3 vec2);
void vec3_normalize(Vec3 *arr);

#endif
