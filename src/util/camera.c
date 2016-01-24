#include <stdlib.h>
#include "camera.h"

extern Mat4 MAT4_IDENT;

void camera_init(Camera *camera, int width, int height){
	camera->location.x = 0;
    camera->location.y = 10;
    camera->location.z = -10;

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

    camera->speed = 25.0;

	camera->follow_target = NULL;

    camera_update_view_matrix(camera);
    camera_update_projection_matrix(camera);
}

void camera_set_follow(Camera *camera, MoveableObject *follow){
	camera->follow_target = follow;
}

void camera_follow(Camera *camera){
	if (camera->follow_target == NULL) return;
	camera->look_at = camera->follow_target->position;
	Vec3 behind = vec3_scale(camera->follow_target->direction, -15);
	behind.y += 7;
	camera->location = vec3_add(camera->follow_target->position, behind);
	camera_update_view_matrix(camera);
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

    sideways = vec3_cross(forwards, camera.up);
    vec3_normalize(&sideways);

    return sideways;
}

void camera_move_forwards(Camera *camera, double dt, float direction){
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

void camera_strafe(Camera *camera, double dt, float direction){
    int i;
    Vec3 forwards, sideways;

    for (i = 0; i < 3; i++) {
        forwards.data[i] = camera->look_at.data[i] - camera->location.data[i];
    }
    vec3_normalize(&forwards);

    sideways = vec3_cross(forwards, camera->up);
    vec3_normalize(&sideways);

    for (i = 0; i < 3; i++) {
        camera->look_at.data[i] += camera->speed * direction * dt * sideways.data[i];
        camera->location.data[i] += camera->speed * direction * dt * sideways.data[i];
    }
}

void camera_rotate_view(Camera *camera, double side_amt, double up_amt){
    int i;
    Vec3 forwards, sideways;

    for (i = 0; i < 3; i++) {
        forwards.data[i] = camera->look_at.data[i] - camera->location.data[i];
    }
    vec3_normalize(&forwards);

    sideways = vec3_cross(forwards, camera->up);
    vec3_normalize(&sideways);

    for (i = 0; i < 3; i++)
        camera->look_at.data[i] += 5.0 * side_amt * sideways.data[i] + 5.0 * up_amt * camera->up.data[i];
}


void camera_move_vertically(Camera *camera, double dt, float direction){
    camera->look_at.data[1] += camera->speed * direction * dt;
    camera->location.data[1] += camera->speed * direction * dt;
}

void camera_update_view_matrix(Camera *camera){
	mat4_ident(&camera->view_matrix);
    mat4_lookat(&camera->view_matrix, camera->location, camera->look_at, camera->up);
}

void camera_update_projection_matrix(Camera *camera){
    mat4_ident(&camera->projection_matrix);
    mat4_persp(&camera->projection_matrix, camera->fov, camera->aspect_ratio, camera->z_near, camera->z_far);
}

void camera_handle_inputs(Camera *camera, double dt, Inputs inputs){
	if (camera->follow_target != NULL) return;

	if (inputs.space_down) { //move vertically
        double dir = inputs.space_shift_down ? -1.0 : 1.0;
        camera_move_vertically(camera, dt, dir);
    }

    if (inputs.left_mouse_down) {
        double dx = inputs.mouse_vel[0] / 100.0;
        double dy = -inputs.mouse_vel[1] / 100.0;
        camera_rotate_view(camera, dx, dy);
    }

    if (inputs.w_down) camera_move_forwards(camera, dt, 1.0);
    if (inputs.s_down) camera_move_forwards(camera, dt, -1.0);
    if (inputs.a_down) camera_strafe(camera, dt, -1.0);
    if (inputs.d_down) camera_strafe(camera, dt, 1.0);

    camera_update_view_matrix(camera);
}


