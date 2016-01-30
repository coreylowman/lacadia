#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "mat4.h"
#include "inputs.h"
#include "game/moveable_object.h"

typedef struct Camera {
    Mat4 projection_matrix;
    Mat4 view_matrix;

    //view parameters
    Vec3 location;
    Vec3 look_at;
    Vec3 up;

    //perspective projection parameters
    float fov;
    float aspect_ratio;
    float z_near, z_far;

    float speed;

    Vec3 follow_location;
	MoveableObject *follow_target;
} Camera;

void camera_init(Camera *camera, int width, int height);

void camera_set_follow(Camera *camera, MoveableObject *follow);

void camera_follow(Camera *camera);

Vec3 camera_get_forwards(Camera camera);
Vec3 camera_get_sideways(Camera camera);

void camera_move_forwards(Camera *camera, double dt, float direction);
void camera_move_vertically(Camera *camera, double dt, float direction);
void camera_rotate_view(Camera *camera, double side_amt, double up_amt);
void camera_strafe(Camera *camera, double dt, float direction);

void camera_update_view_matrix(Camera *camera);
void camera_update_projection_matrix(Camera *camera);

void camera_handle_inputs(Camera *camera, double dt, Inputs inputs);

#endif
