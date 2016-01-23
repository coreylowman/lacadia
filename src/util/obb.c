#include <math.h>
#include "obb.h"
#include <GL/glew.h>
#include <GL/glfw3.h>

extern Vec3 VEC3_ZERO;

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
    t = vec3_from3f(vec3_dot(t, a.axis[0]), vec3_dot(t, a.axis[1]), vec3_dot(t, a.axis[2]));
   
    Vec3 absR[3];
    for(i = 0; i < 3; i++)
         for(j = 0; j < 3; j++)
            absR[i].data[j] = abs(R[i].data[j]);
   
    float ra, rb;
    // Test the three major axes of this OBB.
    for(i = 0; i < 3; i++){
        ra = a.radius.data[i];
        rb = vec3_dot(b.radius, absR[i]);
        if(abs(t.data[i]) > ra + rb)
            return 0;
    }
   
    // Test the three major axes of the OBB b.
    for(i = 0; i < 3; i++){
        ra = a.radius.x * absR[0].data[i] + a.radius.y * absR[1].data[i] + a.radius.z * absR[2].data[i];
        rb = b.radius.data[i];
        if(abs(t.x * R[0].data[i] + t.y * R[1].data[i] + t.z * R[2].data[i]) > ra + rb)
            return 0;
    }

    // Test the 9 different cross-axes.
   
    // A.x <cross> B.x
    ra = a.radius.y * absR[2].data[0] + a.radius.z * absR[1].data[0];
    rb = b.radius.y * absR[0].data[2] + b.radius.z * absR[0].data[1];
    if(abs(t.z * R[1].data[0] - t.y * R[2].data[0]) > ra + rb)
         return 0;
   
    // A.x < cross> B.y
    ra = a.radius.y * absR[2].data[1] + a.radius.z * absR[1].data[1];
    rb = b.radius.x * absR[0].data[2] + b.radius.z * absR[0].data[0];
    if (abs(t.z * R[1].data[1] - t.y * R[2].data[1]) > ra + rb)
         return 0;
   
    // A.x <cross> B.z
    ra = a.radius.y * absR[2].data[2] + a.radius.z * absR[1].data[2];
    rb = b.radius.x * absR[0].data[1] + b.radius.y * absR[0].data[0];
    if (abs(t.z * R[1].data[2] - t.y * R[2].data[2]) > ra + rb)
         return 0;
   
    // A.y <cross> B.x
    ra = a.radius.x * absR[2].data[0] + a.radius.z * absR[0].data[0];
    rb = b.radius.y * absR[1].data[2] + b.radius.z * absR[1].data[1];
    if (abs(t.x * R[2].data[0] - t.z * R[0].data[0]) > ra + rb)
         return 0;
   
    // A.y <cross> B.y
    ra = a.radius.x * absR[2].data[1] + a.radius.z * absR[0].data[1];
    rb = b.radius.x * absR[1].data[2] + b.radius.z * absR[1].data[0];
    if (abs(t.x * R[2].data[1] - t.z * R[0].data[1]) > ra + rb)
         return 0;
   
    // A.y <cross> B.z
    ra = a.radius.x * absR[2].data[2] + a.radius.z * absR[0].data[2];
    rb = b.radius.x * absR[1].data[1] + b.radius.y * absR[1].data[0];
    if (abs(t.x * R[2].data[2] - t.z * R[0].data[2]) > ra + rb)
         return 0;
   
    // A.z <cross> B.x
    ra = a.radius.x * absR[1].data[0] + a.radius.y * absR[0].data[0];
    rb = b.radius.y * absR[2].data[2] + b.radius.z * absR[2].data[1];
    if (abs(t.y * R[0].data[0] - t.x * R[1].data[0]) > ra + rb)
         return 0;
   
    // A.z <cross> B.y
    ra = a.radius.x * absR[1].data[1] + a.radius.y * absR[0].data[1];
    rb = b.radius.x * absR[2].data[2] + b.radius.z * absR[2].data[0];
    if (abs(t.y * R[0].data[1] - t.x * R[1].data[1]) > ra + rb)
         return 0;
   
    // A.z <cross> B.z
    ra = a.radius.x * absR[1].data[2] + a.radius.y * absR[0].data[2];
    rb = b.radius.x * absR[2].data[1] + b.radius.y * absR[2].data[0];
    if (abs(t.y * R[0].data[2] - t.x * R[1].data[2]) > ra + rb)
         return 0;
   
    // No separating axis exists, so the two OBB don't intersect.
    return 1;
}

void obb_rotate_y(Obb *self, float rads){
	float s = sin(rads), c = cos(rads);

	self->axis[0].data[0] = c;
	self->axis[0].data[1] = 0;
	self->axis[0].data[2] = -s;

	self->axis[2].data[0] = s;
	self->axis[2].data[1] = 0;
	self->axis[2].data[2] = c;
}

void obb_render(Obb a){
    int i, ti;
    Vec3 r = VEC3_ZERO;
    Vec3 rs[3];
    for(i = 0;i < 3;i++) rs[i] = vec3_scale(a.axis[i], 2 * a.radius.data[i]);
    for(i = 0;i < 3;i++) r = vec3_add(r, rs[i]);
	r = vec3_scale(r, 0.5);
	Vec3 max = vec3_add(a.center, r);
	Vec3 min = vec3_sub(a.center, r);

    glBegin(GL_LINES);

	for (i = 0; i < 3; i++){
		ti = (i + 1) % 3;

		glVertex3f(min.x, min.y, min.z);
		glVertex3f(min.x + rs[i].x, min.y + rs[i].y, min.z + rs[i].z);

		glVertex3f(max.x, max.y, max.z);
		glVertex3f(max.x - rs[i].x, max.y - rs[i].y, max.z - rs[i].z);

		glVertex3f(min.x + rs[i].x, min.y + rs[i].y, min.z + rs[i].z);
		glVertex3f(max.x - rs[ti].x, max.y - rs[ti].y, max.z - rs[ti].z);

		glVertex3f(min.x + rs[ti].x, min.y + rs[ti].y, min.z + rs[ti].z);
		glVertex3f(max.x - rs[i].x, max.y - rs[i].y, max.z - rs[i].z);
	}

    glEnd();
}


