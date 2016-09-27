#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "mat4.h"
#include "inputs.h"

typedef struct Camera {
  Mat4 projection_matrix;
  Mat4 view_matrix;

  float speed;
  Vec3 position;
  Vec3 forward;
  Vec3 up;

  // perspective projection parameters
  float fov;
  float aspect_ratio;
  float z_near, z_far;

  Vec3 follow_offset;
  Vec3 *follow_position;
} Camera;

void camera_init(Camera *camera, int width, int height);

int camera_is_following(Camera camera);
void camera_set_follow(Camera *camera, Vec3 *follow_position, Vec3 offset);
void camera_follow(Camera *camera, double dt, Inputs inputs);

Vec3 camera_get_forwards(Camera camera);
Vec3 camera_get_sideways(Camera camera);

void camera_move_forwards(Camera *camera, double dt, float direction);
void camera_move_vertically(Camera *camera, double dt, float direction);
void camera_rotate_view(Camera *camera, float dx_radians, float dy_radians);
void camera_rotate_around_follow(Camera *camera, float dx_radians,
                                 float dy_radians);
void camera_strafe(Camera *camera, double dt, float direction);

void camera_invert_pitch(Camera *camera, float height);

void camera_update_view_matrix(Camera *camera);
void camera_update_projection_matrix(Camera *camera);

void camera_handle_inputs(Camera *camera, double dt, Inputs inputs);

#endif
