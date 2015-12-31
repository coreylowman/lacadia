#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "mat4.h"

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
} Camera;

void camera_init(Camera *camera, int width, int height);

Vec3 camera_get_forwards(Camera camera);
Vec3 camera_get_sideways(Camera camera);

void move_forwards(Camera *camera, double dt, float direction);
void move_vertically(Camera *camera, double dt, float direction);
void rotate_view(Camera *camera, double side_amt, double up_amt);
void strafe(Camera *camera, double dt, float direction);

void update_view_matrix(Camera *camera);
void update_projection_matrix(Camera *camera);

#endif
