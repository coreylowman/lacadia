#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "camera.h"

void camera_init(Camera *camera, int width, int height) {
  camera->location.x = 0;
  camera->location.y = 20;
  camera->location.z = 30;

  camera->look_at.x = 0;
  camera->look_at.y = 0;
  camera->look_at.z = 0;

  camera->up.x = 0;
  camera->up.y = 1;
  camera->up.z = 0;

  camera->fov = 45.0;
  camera->aspect_ratio = (float)width / height;
  camera->z_near = 0.1;
  camera->z_far = 1000.0;

  camera->speed = 25.0;

  camera->follow_dist = 30.0;
  camera->follow_position = NULL;

  camera_update_view_matrix(camera);
  camera_update_projection_matrix(camera);
}

int camera_is_following(Camera camera) {
  return camera.follow_position != NULL;
}

void camera_set_follow(Camera *camera, Vec3 *follow_position, float height) {
  camera->follow_position = follow_position;
  if (follow_position != NULL) {
    camera->target_height = height;
    camera->look_at = *follow_position;
    camera->look_at.y += height;
    Vec3 diff = vec3_sub(camera->location, camera->look_at);
    camera->follow_dist = sqrt(vec3_dot(diff, diff));
  }
}

void camera_follow(Camera *camera, double dt, Inputs inputs) {
  if (camera->follow_position == NULL)
    return;

  // (*camera->follow_position, vec3_scale(camera->follow_position->direction,
  // -camera->follow_dist));
  camera->location.x = camera->follow_position->x;
  camera->location.y = camera->follow_position->y + 20;
  camera->location.z = camera->follow_position->z + 20;

  camera->look_at = *camera->follow_position;
  camera->look_at.y += camera->target_height;

  camera_update_view_matrix(camera);
}

Vec3 camera_get_forwards(Camera camera) {
  Vec3 forwards;
  int i;
  for (i = 0; i < 3; i++) {
    forwards.data[i] = camera.look_at.data[i] - camera.location.data[i];
  }
  vec3_normalize(&forwards);
  return forwards;
}

Vec3 camera_get_sideways(Camera camera) {
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

void camera_move_forwards(Camera *camera, double dt, float direction) {
  int i;
  Vec3 forwards;
  for (i = 0; i < 3; i++) {
    forwards.data[i] = camera->look_at.data[i] - camera->location.data[i];
  }
  vec3_normalize(&forwards);
  for (i = 0; i < 3; i++) {
     camera->look_at.data[i] +=
         camera->speed * direction * dt * forwards.data[i];
    camera->location.data[i] +=
        camera->speed * direction * dt * forwards.data[i];
  }
}

void camera_strafe(Camera *camera, double dt, float direction) {
  int i;
  Vec3 forwards, sideways;

  for (i = 0; i < 3; i++) {
    forwards.data[i] = camera->look_at.data[i] - camera->location.data[i];
  }
  vec3_normalize(&forwards);

  sideways = vec3_cross(forwards, camera->up);
  vec3_normalize(&sideways);

  for (i = 0; i < 3; i++) {
    camera->look_at.data[i] +=
        camera->speed * direction * dt * sideways.data[i];
    camera->location.data[i] +=
        camera->speed * direction * dt * sideways.data[i];
  }
}

void camera_rotate_lookat(Camera *camera, double side_amt, double up_amt) {
  int i;
  Vec3 forwards, sideways;

  for (i = 0; i < 3; i++) {
    forwards.data[i] = camera->look_at.data[i] - camera->location.data[i];
  }
  vec3_normalize(&forwards);

  sideways = vec3_cross(forwards, camera->up);
  vec3_normalize(&sideways);

  for (i = 0; i < 3; i++)
    camera->look_at.data[i] +=
        5.0 * side_amt * sideways.data[i] + 5.0 * up_amt * camera->up.data[i];
}

void camera_rotate_around_lookat(Camera *camera, double side_amt,
                                 double up_amt) {
  int i;
  Vec3 neg_lookat = camera->look_at;
  for (i = 0; i < 3; i++)
    neg_lookat.data[i] = -neg_lookat.data[i];

  Vec3 forwards, sideways;
  for (i = 0; i < 3; i++) {
    forwards.data[i] = camera->look_at.data[i] - camera->location.data[i];
  }
  vec3_normalize(&forwards);

  sideways = vec3_cross(forwards, camera->up);
  vec3_normalize(&sideways);

  Mat4 rotation_matrix;
  mat4_ident(&rotation_matrix);
  mat4_translate(&rotation_matrix, neg_lookat);
  mat4_rotate_y(&rotation_matrix, -side_amt);
  mat4_rotate(&rotation_matrix, -up_amt, sideways);
  mat4_translate(&rotation_matrix, camera->look_at);

  Vec3 location;
  mat4_mul_vec3(&location, rotation_matrix, camera->location);
  camera->location = location;
}

void camera_move_vertically(Camera *camera, double dt, float direction) {
  camera->look_at.data[1] += camera->speed * direction * dt;
  camera->location.data[1] += camera->speed * direction * dt;
}

void camera_update_view_matrix(Camera *camera) {
  mat4_ident(&camera->view_matrix);
  mat4_lookat(&camera->view_matrix, camera->location, camera->look_at,
              camera->up);
}

void camera_update_projection_matrix(Camera *camera) {
  mat4_ident(&camera->projection_matrix);
  mat4_persp(&camera->projection_matrix, camera->fov, camera->aspect_ratio,
             camera->z_near, camera->z_far);
}

static Vec3 reflect(Vec3 d, Vec3 n) {
  float dot = 2 * vec3_dot(d, n);
  return vec3_sub(d, vec3_scale(n, dot));
}

void camera_invert_pitch(Camera *camera, float height) {
  Vec3 forwards = vec3_sub(camera->look_at, camera->location);
  float dist = 2 * (camera->location.y - height);
  camera->location.y -= dist;
  camera->look_at = vec3_add(camera->location, reflect(forwards, VEC3_UNIT_Y));
  camera->up = vec3_scale(camera->up, -1.0);
  camera_update_view_matrix(camera);
}

void camera_handle_inputs(Camera *camera, double dt, Inputs inputs) {
  if (camera->follow_position != NULL)
    return;

  if (inputs.space_down) { // move vertically
    double dir = inputs.space_shift_down ? -1.0 : 1.0;
    camera_move_vertically(camera, dt, dir);
  }

  if (inputs.left_mouse_down) {
	  double dx = (inputs.mouse_pos[0] - inputs.mouse_down_start_pos[0]) / 100;
	  double dy = -(inputs.mouse_pos[1] - inputs.mouse_down_start_pos[1]) / 100;
	  camera_rotate_lookat(camera, dx, dy);
  }

  if (inputs.w_down)
    camera_move_forwards(camera, dt, 1.0);
  if (inputs.s_down)
    camera_move_forwards(camera, dt, -1.0);
  if (inputs.a_down)
    camera_strafe(camera, dt, -1.0);
  if (inputs.d_down)
    camera_strafe(camera, dt, 1.0);

  camera_update_view_matrix(camera);
}
