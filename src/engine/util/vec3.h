#ifndef VEC3_H
#define VEC3_H

typedef union {
  float data[3];
  struct {
    float x, y, z;
  };
} Vec3;

static Vec3 VEC3_UNIT_X = {.data = {1, 0, 0}};
static Vec3 VEC3_UNIT_Y = {.data = {0, 1, 0}};
static Vec3 VEC3_UNIT_Z = {.data = {0, 0, 1}};
static Vec3 VEC3_UNIT_NX = {.data = {-1, 0, 0}};
static Vec3 VEC3_UNIT_NY = {.data = {0, -1, 0}};
static Vec3 VEC3_UNIT_NZ = {.data = {0, 0, -1}};
static Vec3 VEC3_ZERO = {.data = {0, 0, 0}};

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
Vec3 vec3_rotate_around_y(Vec3 vec, float degrees);
Vec3 vec3_mix(Vec3 v1, Vec3 v2, float factor);
Vec3 vec3_normalized(Vec3 arr);
float vec3_length(Vec3 vec);
float vec3_dot(Vec3 vec1, Vec3 vec2);
float vec3_degrees_between(Vec3 vec1, Vec3 vec2);
void vec3_normalize(Vec3 *arr);
int vec3_within_dist(Vec3 a, Vec3 b, float r);

#endif
