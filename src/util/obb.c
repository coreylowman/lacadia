#include <math.h>
#include "obb.h"

extern Vec3 VEC3_ZERO;

Vec3 obb_top(Obb a){
    Vec3 output = a.center;
    output.y += a.radius.y;
    return output;
}

Vec3 obb_front(Obb a){
    Vec3 output = a.center;
    output.x += a.radius.z * a.axis[2].x;
    output.y += a.radius.z * a.axis[2].y;
    output.z += a.radius.z * a.axis[2].z;
    return output;
}

//from
//http://clb.demon.fi/MathGeoLib/nightly/docs/OBB.cpp_code.html#926
int obb_intersects(Obb a, Obb b){
    int i, j;
    Vec3 R[3];

    for(i = 0; i < 3; i++)
         for(j = 0; j < 3; j++)
            R[i].data[j] = vec3_dot(a.axis[i], b.axis[j]);
   
    Vec3 t = vec3_sub(b.center, a.center);
    // Express the translation vector in a's coordinate frame.
    t = vec3_from_3f(vec3_dot(t, a.axis[0]), vec3_dot(t, a.axis[1]), vec3_dot(t, a.axis[2]));

    Vec3 absR[3];
    for(i = 0; i < 3; i++)
         for(j = 0; j < 3; j++)
            absR[i].data[j] = fabs(R[i].data[j]);
   
    float ra, rb;
    // Test the three major axes of this OBB.
    for(i = 0; i < 3; i++){
        ra = a.radius.data[i];
        rb = vec3_dot(b.radius, absR[i]);
        if(fabs(t.data[i]) > ra + rb)
            return 0;
    }
   
    // Test the three major axes of the OBB b.
    for(i = 0; i < 3; i++){
        ra = a.radius.data[0] * absR[0].data[i] + a.radius.data[1] * absR[1].data[i] + a.radius.data[2] * absR[2].data[i];
        rb = b.radius.data[i];
        if(fabs(t.x * R[0].data[i] + t.y * R[1].data[i] + t.z * R[2].data[i]) > ra + rb)
            return 0;
    }

    // Test the 9 different cross-axes.

    // A.x <cross> B.x
    ra = a.radius.data[1] * absR[2].data[0] + a.radius.data[2] * absR[1].data[0];
    rb = b.radius.data[1] * absR[0].data[2] + b.radius.data[2] * absR[0].data[1];
    if(fabs(t.z * R[1].data[0] - t.y * R[2].data[0]) > ra + rb)
        return 0;
   
    // A.x < cross> B.y
    ra = a.radius.data[1] * absR[2].data[1] + a.radius.data[2] * absR[1].data[1];
    rb = b.radius.data[0] * absR[0].data[2] + b.radius.data[2] * absR[0].data[0];
    if (fabs(t.z * R[1].data[1] - t.y * R[2].data[1]) > ra + rb)
        return 0;
   
    // A.x <cross> B.z
    ra = a.radius.data[1] * absR[2].data[2] + a.radius.data[2] * absR[1].data[2];
    rb = b.radius.data[0] * absR[0].data[1] + b.radius.data[1] * absR[0].data[0];
    if (fabs(t.z * R[1].data[2] - t.y * R[2].data[2]) > ra + rb)
        return 0;
   
    // A.y <cross> B.x
    ra = a.radius.data[0] * absR[2].data[0] + a.radius.data[2] * absR[0].data[0];
    rb = b.radius.data[1] * absR[1].data[2] + b.radius.data[2] * absR[1].data[1];
    if (fabs(t.x * R[2].data[0] - t.z * R[0].data[0]) > ra + rb)
        return 0;
   
    // A.y <cross> B.y
    ra = a.radius.data[0] * absR[2].data[1] + a.radius.data[2] * absR[0].data[1];
    rb = b.radius.data[0] * absR[1].data[2] + b.radius.data[2] * absR[1].data[0];
    if (fabs(t.x * R[2].data[1] - t.z * R[0].data[1]) > ra + rb)
        return 0;
   
    // A.y <cross> B.z
    ra = a.radius.data[0] * absR[2].data[2] + a.radius.data[2] * absR[0].data[2];
    rb = b.radius.data[0] * absR[1].data[1] + b.radius.data[1] * absR[1].data[0];
    if (fabs(t.x * R[2].data[2] - t.z * R[0].data[2]) > ra + rb)
        return 0;
   
    // A.z <cross> B.x
    ra = a.radius.data[0] * absR[1].data[0] + a.radius.data[1] * absR[0].data[0];
    rb = b.radius.data[1] * absR[2].data[2] + b.radius.data[2] * absR[2].data[1];
    if (fabs(t.y * R[0].data[0] - t.x * R[1].data[0]) > ra + rb)
        return 0;
   
    // A.z <cross> B.y
    ra = a.radius.data[0] * absR[1].data[1] + a.radius.data[1] * absR[0].data[1];
    rb = b.radius.data[0] * absR[2].data[2] + b.radius.data[2] * absR[2].data[0];
    if (fabs(t.y * R[0].data[1] - t.x * R[1].data[1]) > ra + rb)
        return 0;
   
    // A.z <cross> B.z
    ra = a.radius.data[0] * absR[1].data[2] + a.radius.data[1] * absR[0].data[2];
    rb = b.radius.data[0] * absR[2].data[1] + b.radius.data[1] * absR[2].data[0];
    if (fabs(t.y * R[0].data[2] - t.x * R[1].data[2]) > ra + rb)
        return 0;
   
    // No separating axis exists, so the two OBB don't intersect.
    return 1;
}

void obb_rotate_y(Obb *self, float rads){
	float s = sin(rads), c = cos(rads);

	self->axis[0].data[0] = c;
	self->axis[0].data[1] = 0;
	self->axis[0].data[2] = -s;

    self->axis[1].data[0] = 0;
    self->axis[1].data[1] = 1;
    self->axis[1].data[2] = 0;

	self->axis[2].data[0] = s;
	self->axis[2].data[1] = 0;
	self->axis[2].data[2] = c;
}


