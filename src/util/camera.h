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
    float zNear, zFar;

    float speed;
} Camera;

void camera_init(Camera *camera, int width, int height);

void get_forwards_dir(Camera camera, Vec3 *out_dir);
void get_side_dir(Camera camera, Vec3 *out_dir);

void move_forwards(Camera *camera, double dt, float direction);
void strafe(Camera *camera, double dt, float direction);
void rotate_view(Camera *camera, double side_amt, double up_amt);
void move_vertically(Camera *camera, double dt, float direction);

void update_view_matrix(Camera *camera);
void update_projection_matrix(Camera *camera);

#endif
