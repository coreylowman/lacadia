#include <math.h>
#include "vec3.h"

Vec3 VEC3_UNIT_X = { .data = {1, 0, 0} };
Vec3 VEC3_UNIT_Y = { .data = {0, 1, 0} };
Vec3 VEC3_UNIT_Z = { .data = {0, 0, 1} };

Vec3 vec3_from3f(float x, float y, float z){
    return (Vec3){ .x = x, .y = y, .z = z};
}

Vec3 vec3_add(Vec3 v1, Vec3 v2){
    Vec3 out;
    out.x = v1.x + v2.x;
    out.y = v1.y + v2.y;
    out.z = v1.z + v2.z;
    return out;
}

Vec3 vec3_sub(Vec3 v1, Vec3 v2){
	Vec3 out;
	out.x = v1.x - v2.x;
	out.y = v1.y - v2.y;
	out.z = v1.z - v2.z;
	return out;
}

Vec3 vec3_cross(Vec3 vec1, Vec3 vec2){
    Vec3 out;
    out.data[0] = vec1.data[1] * vec2.data[2] - vec1.data[2] * vec2.data[1];
    out.data[1] = vec1.data[2] * vec2.data[0] - vec1.data[0] * vec2.data[2];
    out.data[2] = vec1.data[0] * vec2.data[1] - vec1.data[1] * vec2.data[0];
    return out;
}

Vec3 vec3_scale(Vec3 v, float scale){
    Vec3 out;
    out.x = v.x * scale;
    out.y = v.y * scale;
    out.z = v.z * scale;
    return out;
}

float vec3_dot(Vec3 vec1, Vec3 vec2){
    float sum = 0;
    int i;
    for(i = 0;i < 3;i++){
        sum += vec1.data[i] * vec2.data[i];
    }
    return sum;
}

void vec3_normalize(Vec3 *self){
    float x = self->x;
    float y = self->y;
    float z = self->z;
    float length = sqrt(x * x + y * y + z * z);
    float inv_length = 1 / length;
    self->x = x * inv_length;
    self->y = y * inv_length;
    self->z = z * inv_length;
}
