#include "camera.h"

extern Mat4 MAT4_IDENT;

void camera_init(Camera *camera, int width, int height){
	camera->location.x = 0;
    camera->location.y = 10;
    camera->location.z = 10;

    camera->look_at.x = 0;
    camera->look_at.y = 0;
    camera->look_at.z = 0;

    camera->up.x = 0;
    camera->up.y = 1;
    camera->up.z = 0;
	
    camera->fov = 45.0;
    camera->aspect_ratio = (float) width / height;
    camera->z_near = 0.1;
    camera->z_far = 100.0;

    camera->speed = 5.0;

    update_view_matrix(camera);
    update_projection_matrix(camera);
}

Vec3 camera_get_forwards(Camera camera){
    Vec3 forwards;
	int i;
	for (i = 0; i < 3; i++) {
        forwards.data[i] = camera.look_at.data[i] - camera.location.data[i];
    }
    vec3_normalize(&forwards);
    return forwards;
}

Vec3 camera_get_sideways(Camera camera){
    int i;
    Vec3 forwards, sideways;

    for (i = 0; i < 3; i++) {
        forwards.data[i] = camera.look_at.data[i] - camera.location.data[i];
    }
    vec3_normalize(&forwards);

    vec3_cross(&sideways, forwards, camera.up);
    vec3_normalize(&sideways);

    return sideways;
}

void move_forwards(Camera *camera, double dt, float direction){
    int i;
    Vec3 forwards;
    for (i = 0; i < 3; i++) {
        forwards.data[i] = camera->look_at.data[i] - camera->location.data[i];
    }
    vec3_normalize(&forwards);
    for (i = 0; i < 3; i++) {
        camera->look_at.data[i] += camera->speed * direction * dt * forwards.data[i];
        camera->location.data[i] += camera->speed * direction * dt * forwards.data[i];
    }
}

void strafe(Camera *camera, double dt, float direction){
    int i;
    Vec3 forwards, sideways;

    for (i = 0; i < 3; i++) {
        forwards.data[i] = camera->look_at.data[i] - camera->location.data[i];
    }
    vec3_normalize(&forwards);

    vec3_cross(&sideways, forwards, camera->up);
    vec3_normalize(&sideways);

    for (i = 0; i < 3; i++) {
        camera->look_at.data[i] += camera->speed * direction * dt * sideways.data[i];
        camera->location.data[i] += camera->speed * direction * dt * sideways.data[i];
    }
}

void rotate_view(Camera *camera, double side_amt, double up_amt){
    int i;
    Vec3 forwards, sideways;

    for (i = 0; i < 3; i++) {
        forwards.data[i] = camera->look_at.data[i] - camera->location.data[i];
    }
    vec3_normalize(&forwards);

    vec3_cross(&sideways, forwards, camera->up);
    vec3_normalize(&sideways);

    for (i = 0; i < 3; i++)
        camera->look_at.data[i] += side_amt * sideways.data[i] + up_amt * camera->up.data[i];
}


void move_vertically(Camera *camera, double dt, float direction){
    camera->look_at.data[1] += camera->speed * direction * dt;
    camera->location.data[1] += camera->speed * direction * dt;
}

void update_view_matrix(Camera *camera){
	mat4_ident(&camera->view_matrix);
    mat4_lookat(&camera->view_matrix, camera->location, camera->look_at, camera->up);
}

void update_projection_matrix(Camera *camera){
    mat4_ident(&camera->projection_matrix);
    mat4_persp(&camera->projection_matrix, camera->fov, camera->aspect_ratio, camera->z_near, camera->z_far);
}
