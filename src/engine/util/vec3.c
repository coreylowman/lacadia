#include <math.h>
#include "vec3.h"

Vec3 vec3_from_3f(float x, float y, float z) {
  return (Vec3){.x = x, .y = y, .z = z};
}

Vec3 vec3_add(Vec3 v1, Vec3 v2) {
  Vec3 out;
  out.x = v1.x + v2.x;
  out.y = v1.y + v2.y;
  out.z = v1.z + v2.z;
  return out;
}

Vec3 vec3_sub(Vec3 v1, Vec3 v2) {
  Vec3 out;
  out.x = v1.x - v2.x;
  out.y = v1.y - v2.y;
  out.z = v1.z - v2.z;
  return out;
}

Vec3 vec3_cross(Vec3 vec1, Vec3 vec2) {
  Vec3 out;
  out.data[0] = vec1.data[1] * vec2.data[2] - vec1.data[2] * vec2.data[1];
  out.data[1] = vec1.data[2] * vec2.data[0] - vec1.data[0] * vec2.data[2];
  out.data[2] = vec1.data[0] * vec2.data[1] - vec1.data[1] * vec2.data[0];
  return out;
}

Vec3 vec3_scale(Vec3 v, float scale) {
  Vec3 out;
  out.x = v.x * scale;
  out.y = v.y * scale;
  out.z = v.z * scale;
  return out;
}

Vec3 vec3_rotate_around_y(Vec3 vec, float degrees) {
  double radians = degrees * 3.14159265358979323846 / 180;
  double sin_t = sin(radians);
  double cos_t = cos(radians);
  float x = cos_t * vec.x - sin_t * vec.z;
  float z = sin_t * vec.x + cos_t * vec.z;
  return vec3_from_3f(x, vec.y, z);
}

Vec3 vec3_mix(Vec3 v1, Vec3 v2, float factor) {
  Vec3 out;
  out.x = v1.x * factor + v2.x * (1 - factor);
  out.y = v1.y * factor + v2.y * (1 - factor);
  out.z = v1.z * factor + v2.z * (1 - factor);
  return out;
}

Vec3 vec3_normalized(Vec3 arr) {
  vec3_normalize(&arr);
  return arr;
}

Vec3 vec3_reflect(Vec3 v, Vec3 normal) {
  return vec3_sub(v, vec3_scale(normal, 2 * vec3_dot(v, normal)));
}

float vec3_length(Vec3 vec) {
  return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float vec3_dot(Vec3 vec1, Vec3 vec2) {
  float sum = 0;
  int i;
  for (i = 0; i < 3; i++) {
    sum += vec1.data[i] * vec2.data[i];
  }
  return sum;
}

float vec3_degrees_between(Vec3 vec1, Vec3 vec2) {
  float dot = vec3_dot(vec1, vec2);
  float mag = vec3_length(vec2) * vec3_length(vec1);
  return acos((double)(dot / mag)) * 180 / 3.14159265358979323846;
}

void vec3_normalize(Vec3 *self) {
  float x = self->x;
  float y = self->y;
  float z = self->z;
  float length = sqrt(x * x + y * y + z * z);
  float inv_length = 1 / length;
  self->x = x * inv_length;
  self->y = y * inv_length;
  self->z = z * inv_length;
}

int vec3_within_dist(Vec3 a, Vec3 b, float r) {
  float r2 = r * r;
  Vec3 a_b = vec3_sub(a, b);
  return vec3_dot(a_b, a_b) <= r2;
}
