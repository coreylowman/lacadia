#ifndef MAT4_H
#define MAT4_H

#include "vec3.h"

typedef union {
    float data[16];
    float mat[4][4];
    struct {
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;
    };
    struct {
        float x, y, z, w;
    } vec[4];
} Mat4;

void mat4_ident(Mat4 *mat);

void mat4_mul(Mat4 *dest, Mat4 mat1, Mat4 mat2);

void mat4_scale(Mat4 *mat, float scale);
void mat4_scale3(Mat4 *mat, float *scale);
void mat4_translate(Mat4 *mat, Vec3 pos);

void mat4_rotate(Mat4 *mat, Vec3 direction);
void mat4_rotate_x(Mat4 *mat, float rads);
void mat4_rotate_y(Mat4 *mat, float rads);
void mat4_rotate_z(Mat4 *mat, float rads);

void mat4_copy(Mat4 *dest, Mat4 src);

void mat4_persp(Mat4 *mat, float fov, float aspect, float zNear, float zFar);

void mat4_lookat(Mat4 *mat,Vec3 eye, Vec3 center, Vec3 up);

int mat4_inverse(Mat4 *out_mat, Mat4 mat);

void mat4_mul_vec4(Vec3 *out_vec, Mat4 mat, Vec3 vec);

#endif
