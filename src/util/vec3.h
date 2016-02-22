#ifndef VEC3_H
#define VEC3_H

typedef union {
    float data[3];
    struct {
        float x, y, z;
    };
} Vec3;

typedef union {
    int data[3];
    struct {
        int x, y, z;
    };
} Vec3_i;

Vec3 vec3_from_3f(float x, float y, float z);
Vec3 vec3_add(Vec3 v1, Vec3 v2);
Vec3 vec3_sub(Vec3 v1, Vec3 v2);
Vec3 vec3_scale(Vec3 v, float scale);
Vec3 vec3_cross(Vec3 vec1, Vec3 vec2);
float vec3_dot(Vec3 vec1, Vec3 vec2);
void vec3_normalize(Vec3 *arr);
int vec3_within_dist(Vec3 a, Vec3 b, float r);

#endif
