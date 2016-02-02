#include <math.h>
#include "mat4.h"

Mat4 MAT4_IDENT = {
    .m00 = 1, .m01 = 0, .m02 = 0, .m03 = 0,
    .m10 = 0, .m11 = 1, .m12 = 0, .m13 = 0,
    .m20 = 0, .m21 = 0, .m22 = 1, .m23 = 0,
    .m30 = 0, .m31 = 0, .m32 = 0, .m33 = 1,
};

void mat4_ident(Mat4 *mat){
	*mat = MAT4_IDENT;
}

void mat4_mul(Mat4 *dest, Mat4 mat1, Mat4 mat2){
    float a00 = mat1.data[0], a01 = mat1.data[1], a02 = mat1.data[2], a03 = mat1.data[3],
    a10 = mat1.data[4], a11 = mat1.data[5], a12 = mat1.data[6], a13 = mat1.data[7],
    a20 = mat1.data[8], a21 = mat1.data[9], a22 = mat1.data[10], a23 = mat1.data[11],
    a30 = mat1.data[12], a31 = mat1.data[13], a32 = mat1.data[14], a33 = mat1.data[15],

    b00 = mat2.data[0], b01 = mat2.data[1], b02 = mat2.data[2], b03 = mat2.data[3],
    b10 = mat2.data[4], b11 = mat2.data[5], b12 = mat2.data[6], b13 = mat2.data[7],
    b20 = mat2.data[8], b21 = mat2.data[9], b22 = mat2.data[10], b23 = mat2.data[11],
    b30 = mat2.data[12], b31 = mat2.data[13], b32 = mat2.data[14], b33 = mat2.data[15];

    dest->data[0] = b00 * a00 + b01 * a10 + b02 * a20 + b03 * a30;
    dest->data[1] = b00 * a01 + b01 * a11 + b02 * a21 + b03 * a31;
    dest->data[2] = b00 * a02 + b01 * a12 + b02 * a22 + b03 * a32;
    dest->data[3] = b00 * a03 + b01 * a13 + b02 * a23 + b03 * a33;
    dest->data[4] = b10 * a00 + b11 * a10 + b12 * a20 + b13 * a30;
    dest->data[5] = b10 * a01 + b11 * a11 + b12 * a21 + b13 * a31;
    dest->data[6] = b10 * a02 + b11 * a12 + b12 * a22 + b13 * a32;
    dest->data[7] = b10 * a03 + b11 * a13 + b12 * a23 + b13 * a33;
    dest->data[8] = b20 * a00 + b21 * a10 + b22 * a20 + b23 * a30;
    dest->data[9] = b20 * a01 + b21 * a11 + b22 * a21 + b23 * a31;
    dest->data[10] = b20 * a02 + b21 * a12 + b22 * a22 + b23 * a32;
    dest->data[11] = b20 * a03 + b21 * a13 + b22 * a23 + b23 * a33;
    dest->data[12] = b30 * a00 + b31 * a10 + b32 * a20 + b33 * a30;
    dest->data[13] = b30 * a01 + b31 * a11 + b32 * a21 + b33 * a31;
    dest->data[14] = b30 * a02 + b31 * a12 + b32 * a22 + b33 * a32;
    dest->data[15] = b30 * a03 + b31 * a13 + b32 * a23 + b33 * a33;
}

void mat4_scale(Mat4 *mat, float scale){
    mat->data[0] = mat->data[0] * scale;
    mat->data[1] = mat->data[1] * scale;
    mat->data[2] = mat->data[2] * scale;
    mat->data[3] = mat->data[3] * scale;
    mat->data[4] = mat->data[4] * scale;
    mat->data[5] = mat->data[5] * scale;
    mat->data[6] = mat->data[6] * scale;
    mat->data[7] = mat->data[7] * scale;
    mat->data[8] = mat->data[8] * scale;
    mat->data[9] = mat->data[9] * scale;
    mat->data[10] = mat->data[10] * scale;
    mat->data[11] = mat->data[11] * scale;
    mat->data[12] = mat->data[12];
    mat->data[13] = mat->data[13];
    mat->data[14] = mat->data[14];
    mat->data[15] = mat->data[15];
}

void mat4_scale3(Mat4 *mat, float *scale){
    float x = scale[0], y = scale[1], z = scale[2];
    mat->data[0] = mat->data[0] * x;
    mat->data[1] = mat->data[1] * x;
    mat->data[2] = mat->data[2] * x;
    mat->data[3] = mat->data[3] * x;
    mat->data[4] = mat->data[4] * y;
    mat->data[5] = mat->data[5] * y;
    mat->data[6] = mat->data[6] * y;
    mat->data[7] = mat->data[7] * y;
    mat->data[8] = mat->data[8] * z;
    mat->data[9] = mat->data[9] * z;
    mat->data[10] = mat->data[10] * z;
    mat->data[11] = mat->data[11] * z;
    mat->data[12] = mat->data[12];
    mat->data[13] = mat->data[13];
    mat->data[14] = mat->data[14];
    mat->data[15] = mat->data[15];
}

void mat4_translate(Mat4 *mat, Vec3 pos){
    float x = pos.data[0], y = pos.data[1], z = pos.data[2],
        a00, a01, a02, a03,
        a10, a11, a12, a13,
        a20, a21, a22, a23;

    a00 = mat->data[0]; a01 = mat->data[1]; a02 = mat->data[2]; a03 = mat->data[3];
    a10 = mat->data[4]; a11 = mat->data[5]; a12 = mat->data[6]; a13 = mat->data[7];
    a20 = mat->data[8]; a21 = mat->data[9]; a22 = mat->data[10]; a23 = mat->data[11];

    mat->data[12] += a00 * x + a10 * y + a20 * z;
    mat->data[13] += a01 * x + a11 * y + a21 * z;
    mat->data[14] += a02 * x + a12 * y + a22 * z;
    mat->data[15] += a03 * x + a13 * y + a23 * z;
}

void mat4_rotate(Mat4 *mat, Vec3 direction){
    mat4_rotate_x(mat, direction.x);
    mat4_rotate_y(mat, direction.y);
    mat4_rotate_z(mat, direction.z);
}

void mat4_rotate_x(Mat4 *mat, float rads){
    float s = sin(rads), c = cos(rads), a10 = mat->data[4], a11 = mat->data[5],
        a12 = mat->data[6], a13 = mat->data[7], a20 = mat->data[8],
        a21 = mat->data[9], a22 = mat->data[10], a23 = mat->data[11];

    mat->data[4] = a10 * c + a20 * s;
    mat->data[5] = a11 * c + a21 * s;
    mat->data[6] = a12 * c + a22 * s;
    mat->data[7] = a13 * c + a23 * s;

    mat->data[8] = a10 * -s + a20 * c;
    mat->data[9] = a11 * -s + a21 * c;
    mat->data[10] = a12 * -s + a22 * c;
    mat->data[11] = a13 * -s + a23 * c;
}

void mat4_rotate_y(Mat4 *mat, float rads){
    float s = sin(rads), c = cos(rads), a00 = mat->data[0], a01 = mat->data[1],
        a02 = mat->data[2], a03 = mat->data[3], a20 = mat->data[8],
        a21 = mat->data[9], a22 = mat->data[10], a23 = mat->data[11];

    mat->data[0] = a00 * c + a20 * -s;
    mat->data[1] = a01 * c + a21 * -s;
    mat->data[2] = a02 * c + a22 * -s;
    mat->data[3] = a03 * c + a23 * -s;

    mat->data[8] = a00 * s + a20 * c;
    mat->data[9] = a01 * s + a21 * c;
    mat->data[10] = a02 * s + a22 * c;
    mat->data[11] = a03 * s + a23 * c;
}

void mat4_rotate_z(Mat4 *mat, float rads){
    float s = sin(rads), c = cos(rads), a00 = mat->data[0], a01 = mat->data[1],
        a02 = mat->data[2], a03 = mat->data[3], a10 = mat->data[4],
        a11 = mat->data[5], a12 = mat->data[6], a13 = mat->data[7];

    mat->data[0] = a00 * c + a10 * s;
    mat->data[1] = a01 * c + a11 * s;
    mat->data[2] = a02 * c + a12 * s;
    mat->data[3] = a03 * c + a13 * s;

    mat->data[4] = a00 * -s + a10 * c;
    mat->data[5] = a01 * -s + a11 * c;
    mat->data[6] = a02 * -s + a12 * c;
    mat->data[7] = a03 * -s + a13 * c;
}

void mat4_copy(Mat4 *dest, Mat4 src){
    int i;
    for(i = 0;i < 16;i++)
        dest->data[i] = src.data[i];
}

static float radians(float degrees){
	return degrees * 3.14159265 / 180.0;
}

void mat4_persp(Mat4 *mat, float fov, float aspect, float zNear, float zFar){
    float tanHalf = tan(radians(fov) / 2.0);    

    mat->data[0] = 1 / (aspect * tanHalf); mat->data[1] = 0; mat->data[2] = 0; mat->data[3] = 0;
    mat->data[4] = 0; mat->data[5] = 1 / tanHalf; mat->data[6] = 0; mat->data[7] = 0;
    mat->data[8] = 0; mat->data[9] = 0; mat->data[10] = -(zFar + zNear) / (zFar - zNear); mat->data[11] = -(2 * zFar * zNear) / (zFar - zNear);
    mat->data[12] = 0; mat->data[13] = 0; mat->data[14] = -1; mat->data[15] = 1;
}

void mat4_lookat(Mat4 *mat, Vec3 eye, Vec3 center, Vec3 up){
    int i;
    Vec3 f, s, u;

    for(i = 0;i < 3;i++){
        f.data[i] = center.data[i] - eye.data[i];
    }
    vec3_normalize(&f);

    s = vec3_cross(f, up);
    vec3_normalize(&s);

    u = vec3_cross(s, f);

	mat->data[0] = s.data[0];
    mat->data[1] = s.data[1];
    mat->data[2] = s.data[2];
    mat->data[3] = -vec3_dot(s, eye);

    mat->data[4] = u.data[0];
    mat->data[5] = u.data[1];
    mat->data[6] = u.data[2];
    mat->data[7] = -vec3_dot(u, eye);

    mat->data[8] = -f.data[0];
    mat->data[9] = -f.data[1];
    mat->data[10] = -f.data[2];
    mat->data[11] = vec3_dot(f, eye);
}

int mat4_inverse(Mat4 *out_mat, Mat4 self){
	float inv[16], det;
	int i;

	float m[16];
	for (i = 0; i < 16; i++)
		m[i] = self.data[i];

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		return 0;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		out_mat->data[i] = inv[i] * det;

	return 1;
}

void mat4_mul_vec3(Vec3 *out_vec, Mat4 mat, Vec3 vec){
    int i;
	float x = vec.x, y = vec.y, z = vec.z;
	out_vec->x = mat.data[0] * x + mat.data[1] * y + mat.data[2] * z + mat.data[3];
	out_vec->y = mat.data[4] * x + mat.data[5] * y + mat.data[6] * z + mat.data[7];
	out_vec->z = mat.data[8] * x + mat.data[9] * y + mat.data[10] * z + mat.data[11];
}

void mat4_transpose(Mat4 *mat){
    Mat4 output;
    output.m00 = mat->m00;
    output.m10 = mat->m01;
    output.m20 = mat->m02;
    output.m30 = mat->m03;

    output.m01 = mat->m10;
    output.m11 = mat->m11;
    output.m21 = mat->m12;
    output.m31 = mat->m13;

    output.m02 = mat->m20;
    output.m12 = mat->m21;
    output.m22 = mat->m22;
    output.m32 = mat->m23;

    output.m03 = mat->m30;
    output.m13 = mat->m31;
    output.m23 = mat->m32;
    output.m33 = mat->m33;

    int i;
    for(i = 0;i < 16;i++){
        mat->data[i] = output.data[i];
    }
}

