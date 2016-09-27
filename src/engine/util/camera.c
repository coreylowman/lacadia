#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "camera.h"

void camera_init(Camera *camera, int width, int height) {
  camera->speed = 50;

  camera->position.x = 0;
  camera->position.y = 0;
  camera->position.z = 0;

  camera->forward.x = 0;
  camera->forward.y = 0;
  camera->forward.z = -1;

  camera->up.x = 0;
  camera->up.y = 1;
  camera->up.z = 0;

  camera->fov = 45.0;
  camera->aspect_ratio = (float)width / height;
  camera->z_near = 0.1;
  camera->z_far = 1000.0;

  camera->follow_offset = VEC3_ZERO;
  camera->follow_position = NULL;

  camera_update_view_matrix(camera);
  camera_update_projection_matrix(camera);
}

int camera_is_following(Camera camera) {
  return camera.follow_position != NULL;
}

void camera_set_follow(Camera *camera, Vec3 *follow_position, Vec3 offset) {
  camera->follow_position = follow_position;
  camera->follow_offset = offset;
}

void camera_follow(Camera *camera, double dt, Inputs inputs) {
  if (camera->follow_position == NULL)
    return;

  camera->position = vec3_add(*camera->follow_position, camera->follow_offset);
  camera->forward = vec3_normalized(vec3_scale(camera->follow_offset, -1));
  camera->up.x = camera->forward.x;
  camera->up.y = -(camera->forward.x * camera->forward.x + camera->forward.z * camera->forward.z) / camera->forward.y;
  camera->up.z = camera->forward.z;
  vec3_normalize(&camera->up);

  camera_update_view_matrix(camera);
}

Vec3 camera_get_forwards(Camera camera) {
  return camera.forward;
}

Vec3 camera_get_sideways(Camera camera) {
  return vec3_normalized(vec3_cross(camera.forward, camera.up));
}

void camera_move_forwards(Camera *camera, double dt, float direction) {
  int i;
  for (i = 0; i < 3; i++) {
    camera->position.data[i] += camera->speed * direction * dt * camera->forward.data[i];
  }
}

void camera_strafe(Camera *camera, double dt, float direction) {
  int i;
  Vec3 sideways = vec3_normalized(vec3_cross(camera->forward, camera->up));

  for (i = 0; i < 3; i++) {
    camera->position.data[i] += camera->speed * direction * dt * sideways.data[i];
  }
}

void camera_rotate_view(Camera *camera, float dx_radians, float dy_radians) {
	{
		Mat4 rotation_matrix = MAT4_IDENT;
		mat4_rotate(&rotation_matrix, dx_radians, VEC3_UNIT_Y);

		mat4_mul_vec3(&camera->forward, rotation_matrix, camera->forward);
		vec3_normalize(&camera->forward);

		mat4_mul_vec3(&camera->up, rotation_matrix, camera->up);
		vec3_normalize(&camera->up);
	}

	{
		Vec3 sideways = vec3_normalized(vec3_cross(camera->forward, camera->up));
		Mat4 rotation_matrix = MAT4_IDENT;
		mat4_rotate(&rotation_matrix, dy_radians, sideways);

		mat4_mul_vec3(&camera->forward, rotation_matrix, camera->forward);
		vec3_normalize(&camera->forward);

		mat4_mul_vec3(&camera->up, rotation_matrix, camera->up);
		vec3_normalize(&camera->up);
	}
}

// todo make sure you can't rotate too low/high?
void camera_rotate_around_follow(Camera *camera, float dx_radians, float dy_radians) {
  {
    Mat4 rotation_matrix = MAT4_IDENT;
    mat4_rotate(&rotation_matrix, dx_radians, VEC3_UNIT_Y);

    mat4_mul_vec3(&camera->follow_offset, rotation_matrix, camera->follow_offset);

    camera->position = vec3_add(*camera->follow_position, camera->follow_offset);
    camera->forward = vec3_normalized(vec3_scale(camera->follow_offset, -1));

    mat4_mul_vec3(&camera->up, rotation_matrix, camera->up);
    vec3_normalize(&camera->up);
  }
  {
    Vec3 sideways = vec3_normalized(vec3_cross(camera->forward, camera->up));
    Mat4 rotation_matrix = MAT4_IDENT;
    mat4_rotate(&rotation_matrix, dy_radians, sideways);

    mat4_mul_vec3(&camera->follow_offset, rotation_matrix, camera->follow_offset);
    camera->position = vec3_add(*camera->follow_position, camera->follow_offset);
    camera->forward = vec3_normalized(vec3_scale(camera->follow_offset, -1));

    mat4_mul_vec3(&camera->up, rotation_matrix, camera->up);
    vec3_normalize(&camera->up);
  }
}

void camera_move_vertically(Camera *camera, double dt, float direction) {
  camera->position.data[1] += camera->speed * direction * dt;
}

void camera_update_view_matrix(Camera *camera) {
  mat4_ident(&camera->view_matrix);
  mat4_lookat(&camera->view_matrix, camera->position, camera->forward,
              camera->up);
}

void camera_update_projection_matrix(Camera *camera) {
  mat4_ident(&camera->projection_matrix);
  mat4_persp(&camera->projection_matrix, camera->fov, camera->aspect_ratio,
             camera->z_near, camera->z_far);
}

void camera_invert_pitch(Camera *camera, float height) {
  float dist = 2 * (camera->position.y - height);
  camera->position.y -= dist;
  camera->forward = vec3_normalized(vec3_reflect(camera->forward, VEC3_UNIT_Y));
  camera->up = vec3_normalized(vec3_reflect(camera->up, VEC3_UNIT_Y));
  camera_update_view_matrix(camera);
}

void camera_handle_inputs(Camera *camera, double dt, Inputs inputs) {
  if (camera->follow_position != NULL) {
    if(inputs.left_mouse_down) {
        double dx = (inputs.mouse_pos[0] - inputs.mouse_down_start_pos[0]);
        double dy = (inputs.mouse_pos[1] - inputs.mouse_down_start_pos[1]);
      camera_rotate_around_follow(camera, dt * dx, dt * dy);
    }
  } else {
    if (inputs.space_down) { // move vertically
      double dir = inputs.space_shift_down ? -1.0 : 1.0;
      camera_move_vertically(camera, dt, dir);
    }

    if (inputs.left_mouse_down) {
  	  double dx = (inputs.mouse_pos[0] - inputs.mouse_down_start_pos[0]);
  	  double dy = (inputs.mouse_pos[1] - inputs.mouse_down_start_pos[1]);
	  camera_rotate_view(camera, dx * dt * 0.5, dy * dt * 0.5);
    }

    if (inputs.w_down)
      camera_move_forwards(camera, dt, 1.0);
    if (inputs.s_down)
      camera_move_forwards(camera, dt, -1.0);
    if (inputs.a_down)
      camera_strafe(camera, dt, -1.0);
    if (inputs.d_down)
      camera_strafe(camera, dt, 1.0);
  }

  camera_update_view_matrix(camera);
}
