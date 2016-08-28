#include <math.h>
#include <stdio.h>
#include "mat4.h"

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

    dest->data[0] = a00 * b00 + a01 * b10 + a02 * b20 + a03 * b30;
    dest->data[1] = a00 * b01 + a01 * b11 + a02 * b21 + a03 * b31;
    dest->data[2] = a00 * b02 + a01 * b12 + a02 * b22 + a03 * b32;
    dest->data[3] = a00 * b03 + a01 * b13 + a02 * b23 + a03 * b33;
    dest->data[4] = a10 * b00 + a11 * b10 + a12 * b20 + a13 * b30;
    dest->data[5] = a10 * b01 + a11 * b11 + a12 * b21 + a13 * b31;
    dest->data[6] = a10 * b02 + a11 * b12 + a12 * b22 + a13 * b32;
    dest->data[7] = a10 * b03 + a11 * b13 + a12 * b23 + a13 * b33;
    dest->data[8] = a20 * b00 + a21 * b10 + a22 * b20 + a23 * b30;
    dest->data[9] = a20 * b01 + a21 * b11 + a22 * b21 + a23 * b31;
    dest->data[10] = a20 * b02 + a21 * b12 + a22 * b22 + a23 * b32;
    dest->data[11] = a20 * b03 + a21 * b13 + a22 * b23 + a23 * b33;
    dest->data[12] = a30 * b00 + a31 * b10 + a32 * b20 + a33 * b30;
    dest->data[13] = a30 * b01 + a31 * b11 + a32 * b21 + a33 * b31;
    dest->data[14] = a30 * b02 + a31 * b12 + a32 * b22 + a33 * b32;
    dest->data[15] = a30 * b03 + a31 * b13 + a32 * b23 + a33 * b33;
}

void mat4_scale(Mat4 *mat, float scale){
    mat->data[0] = mat->data[0] * scale;
    mat->data[1] = mat->data[1] * scale;
    mat->data[2] = mat->data[2] * scale;

    mat->data[4] = mat->data[4] * scale;
    mat->data[5] = mat->data[5] * scale;
    mat->data[6] = mat->data[6] * scale;

    mat->data[8] = mat->data[8] * scale;
    mat->data[9] = mat->data[9] * scale;
    mat->data[10] = mat->data[10] * scale;
}

void mat4_translate(Mat4 *mat, Vec3 pos){
    float x = pos.data[0], y = pos.data[1], z = pos.data[2];

    mat->data[3] += x;
    mat->data[7] += y;
    mat->data[11] += z;
}

void mat4_rotate(Mat4 *mat, float rads, Vec3 axis){
    double x = axis.x, y = axis.y, z = axis.z,
        len = sqrt(x * x + y * y + z * z),
        s, c, t,
        a00, a01, a02, a03,
        a10, a11, a12, a13,
        a20, a21, a22, a23,
        b00, b01, b02,
        b10, b11, b12,
        b20, b21, b22;

    if (!len) { return; }
    if (len != 1.0) {
        len = 1 / len;
        x *= len;
        y *= len;
        z *= len;
    }

    s = sin(rads);
    c = cos(rads);
    t = 1 - c;

    a00 = mat->data[0]; a01 = mat->data[1]; a02 = mat->data[2]; a03 = mat->data[3];
    a10 = mat->data[4]; a11 = mat->data[5]; a12 = mat->data[6]; a13 = mat->data[7];
    a20 = mat->data[8]; a21 = mat->data[9]; a22 = mat->data[10]; a23 = mat->data[11];

    // Construct the elements of the rotation matrix
    b00 = x * x * t + c; b01 = y * x * t + z * s; b02 = z * x * t - y * s;
    b10 = x * y * t - z * s; b11 = y * y * t + c; b12 = z * y * t + x * s;
    b20 = x * z * t + y * s; b21 = y * z * t - x * s; b22 = z * z * t + c;

    // Perform rotation-specific matrix multiplication
    mat->data[0] = a00 * b00 + a10 * b01 + a20 * b02;
    mat->data[1] = a01 * b00 + a11 * b01 + a21 * b02;
    mat->data[2] = a02 * b00 + a12 * b01 + a22 * b02;
    mat->data[3] = a03 * b00 + a13 * b01 + a23 * b02;

    mat->data[4] = a00 * b10 + a10 * b11 + a20 * b12;
    mat->data[5] = a01 * b10 + a11 * b11 + a21 * b12;
    mat->data[6] = a02 * b10 + a12 * b11 + a22 * b12;
    mat->data[7] = a03 * b10 + a13 * b11 + a23 * b12;

    mat->data[8] = a00 * b20 + a10 * b21 + a20 * b22;
    mat->data[9] = a01 * b20 + a11 * b21 + a21 * b22;
    mat->data[10] = a02 * b20 + a12 * b21 + a22 * b22;
    mat->data[11] = a03 * b20 + a13 * b21 + a23 * b22;
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

    mat->data[0] = a00 * c + a20 * s;
    mat->data[1] = a01 * c + a21 * s;
    mat->data[2] = a02 * c + a22 * s;
    mat->data[3] = a03 * c + a23 * s;

    mat->data[8] = a00 * -s + a20 * c;
    mat->data[9] = a01 * -s + a21 * c;
    mat->data[10] = a02 * -s + a22 * c;
    mat->data[11] = a03 * -s + a23 * c;
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

void mat4_persp(Mat4 *mat, float fov, float aspect, float zNear, float zFar){
	float tanHalf = tan(fov * 3.14159265358979323846 / 360.0);
    mat->data[0] = 1 / (aspect * tanHalf);
	mat->data[1] = 0;
	mat->data[2] = 0;
	mat->data[3] = 0;

    mat->data[4] = 0;
    mat->data[5] = 1 / tanHalf;
    mat->data[6] = 0;
    mat->data[7] = 0;

    mat->data[8] = 0;
    mat->data[9] = 0;
    mat->data[10] = -(zFar + zNear) / (zFar - zNear);
	mat->data[11] = -(2 * zFar * zNear) / (zFar - zNear);;

    mat->data[12] = 0;
    mat->data[13] = 0;
	mat->data[14] = -1;
    mat->data[15] = 0;
}

void mat4_lookat(Mat4 *mat, Vec3 eye, Vec3 center, Vec3 up){
    int i;
    Vec3 f, s, u;

    for(i = 0;i < 3;i++){
        f.data[i] = center.data[i] - eye.data[i];
    }
	//f is z
    vec3_normalize(&f);

	//s is x
    s = vec3_cross(f, up);
    vec3_normalize(&s);

	//u is y
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

	mat->data[12] = 0;
	mat->data[13] = 0;
	mat->data[14] = 0;
	mat->data[15] = 1;
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

//returns the w component
float mat4_mul_vec3(Vec3 *out_vec, Mat4 mat, Vec3 vec){
	float x = vec.x, y = vec.y, z = vec.z;
	out_vec->x = mat.data[0] * x + mat.data[1] * y + mat.data[2] * z + mat.data[3];
	out_vec->y = mat.data[4] * x + mat.data[5] * y + mat.data[6] * z + mat.data[7];
	out_vec->z = mat.data[8] * x + mat.data[9] * y + mat.data[10] * z + mat.data[11];
	return mat.data[12] * x + mat.data[13] * y + mat.data[14] * z + mat.data[15];
}

void mat4_transpose(Mat4 *mat){
    Mat4 output;

    output.data[0] = mat->data[0];
    output.data[1] = mat->data[4];
    output.data[2] = mat->data[8];
    output.data[3] = mat->data[12];

    output.data[4] = mat->data[1];
    output.data[5] = mat->data[5];
    output.data[6] = mat->data[9];
    output.data[7] = mat->data[13];

    output.data[8] = mat->data[2];
    output.data[9] = mat->data[6];
    output.data[10] = mat->data[10];
    output.data[11] = mat->data[14];

    output.data[12] = mat->data[3];
    output.data[13] = mat->data[7];
    output.data[14] = mat->data[11];
    output.data[15] = mat->data[15];

    int i;
    for(i = 0;i < 16;i++){
        mat->data[i] = output.data[i];
    }
}

