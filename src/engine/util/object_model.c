#include <string.h>
#include <stdio.h>
#include <float.h>
#include "lodepng.h"
#include "object_model.h"
#include "array_list.h"
#include "mat4.h"
#include "vec3.h"

#define OBJECT_MODEL_SCALE 0.25

typedef struct { Vec3 vertex_indices, texture_indices, normal_indices; } Face;

static Face *face_new(Vec3 p, Vec3 t, Vec3 n) {
  Face *self = malloc(sizeof(*self));
  self->vertex_indices = p;
  self->texture_indices = t;
  self->normal_indices = n;
  return self;
}

static void face_free(void *data) { free(data); }

static int load_png_data(const char *filename, unsigned char **image,
                         unsigned *width, unsigned *height) {
  unsigned error;
  unsigned char *png = 0;
  size_t pngsize;

  error = lodepng_load_file(&png, &pngsize, filename);
  if (!error)
    error = lodepng_decode32(image, width, height, png, pngsize);
  if (error) {
    printf("error %u: %s\n", error, lodepng_error_text(error));
    free(png);
    return 0;
  }

  free(png);
  return 1;
}

ObjectModel *obj_model_from_file(const char *filename) {
  ArrayList *face_list = array_list_new(face_free);
  ArrayList_v3 *vertex_list = array_list_new_v3();
  ArrayList_v3 *normal_list = array_list_new_v3();
  ArrayList_v3 *texture_list = array_list_new_v3();

  char line[256] = { 0 };
  char *token;
  Vec3 p, n, t;
  int i;

  char obj_filename[256] = { 0 };
  sprintf(obj_filename, "%s.obj", filename);

  FILE *input = fopen(obj_filename, "r");
  if (input == NULL) {
    printf("error: file %s not opened\n", filename);
  }

  while (fgets(line, 256, input) != NULL) {
    token = strtok(line, " ");
    if (strcmp(token, "vn") == 0) {
      for (i = 0; i < 3; i++) {
        token = strtok(NULL, " ");
        n.data[i] = atof(token);
      }
      array_list_push_v3(normal_list, n);
    } else if (strcmp(token, "vt") == 0) {
      for (i = 0; i < 2; i++) {
        token = strtok(NULL, " ");
        t.data[i] = atof(token);
      }
      t.data[i] = 0;
      array_list_push_v3(texture_list, t);
    } else if (strcmp(token, "v") == 0) {
      for (i = 0; i < 3; i++) {
        token = strtok(NULL, " ");
        p.data[i] = atof(token);
      }
      array_list_push_v3(vertex_list, p);
    } else if (strcmp(token, "f") == 0) {
      for (i = 0; i < 3; i++) {
        token = strtok(NULL, "/");
        p.data[i] = atof(token);
        token = strtok(NULL, "/");
        t.data[i] = atof(token);
        token = strtok(NULL, " ");
        n.data[i] = atof(token);
      }
      array_list_push(face_list, face_new(p, t, n));
    } else if (strcmp(token, "mtllib") == 0) {

    } else if (strcmp(token, "usemtl") == 0) {
    }
  }
  fclose(input);

  unsigned char *png_data;
  unsigned png_width, png_height;
  char png_filename[256] = { 0 };
  sprintf(png_filename, "%s.png", filename);

  if (!load_png_data(png_filename, &png_data, &png_width, &png_height)) {
    printf("error: lodepng error");
  }

  ObjectModel *self = malloc(sizeof(*self));
  self->num_vertices = 3 * face_list->length;
  self->vertices = malloc(self->num_vertices * sizeof(*(self->vertices)));

  Face *f;
  int j, k;
  int tx, ty;
  float inv_255 = 1.0 / 255.0;

  float min[3] = { FLT_MAX }, max[3] = { FLT_MIN };

  ObjectModelVertex objVertex;
  for (i = 0; i < face_list->length; i++) {
    f = face_list->data[i];

    // calc normal
    n = VEC3_ZERO;
    for (j = 0; j < 3; j++) {
      n = vec3_add(n, normal_list->data[(int)f->normal_indices.data[j] - 1]);
    }
    vec3_normalize(&n);

    for (j = 0; j < 3; j++) {
      p = vertex_list->data[(int)f->vertex_indices.data[j] - 1];

      objVertex.position[0] = p.x * OBJECT_MODEL_SCALE;
      objVertex.position[1] = p.y * OBJECT_MODEL_SCALE;
      objVertex.position[2] = p.z * OBJECT_MODEL_SCALE;

      t = texture_list->data[(int)f->texture_indices.data[j] - 1];
      tx = t.x * png_width;
      ty = t.y * png_height;
      objVertex.color[0] =
          png_data[4 * png_width * ty + 4 * tx + 0] * inv_255; // r
      objVertex.color[1] =
          png_data[4 * png_width * ty + 4 * tx + 1] * inv_255; // b
      objVertex.color[2] =
          png_data[4 * png_width * ty + 4 * tx + 2] * inv_255; // g

      objVertex.normal[0] = n.x;
      objVertex.normal[1] = n.y;
      objVertex.normal[2] = n.z;

      self->vertices[3 * i + j] = objVertex;

      for (k = 0; k < 3; k++) {
        if (objVertex.position[k] < min[k])
          min[k] = objVertex.position[k];
        if (objVertex.position[k] > max[k])
          max[k] = objVertex.position[k];
      }
    }
  }

  free(png_data);
  array_list_free(face_list);
  array_list_free(vertex_list);
  array_list_free(texture_list);
  array_list_free(normal_list);

  self->bounding_box.center.x = (max[0] + min[0]) * 0.5;
  self->bounding_box.center.y = (max[1] + min[1]) * 0.5;
  self->bounding_box.center.z = (max[2] + min[2]) * 0.5;

  self->bounding_box.radius.x = (max[0] - min[0]) * 0.5;
  self->bounding_box.radius.y = (max[1] - min[1]) * 0.5;
  self->bounding_box.radius.z = (max[2] - min[2]) * 0.5;

  self->bounding_box.axis[0] = VEC3_UNIT_X;
  self->bounding_box.axis[1] = VEC3_UNIT_Y;
  self->bounding_box.axis[2] = VEC3_UNIT_Z;

  return self;
}

void obj_model_free(ObjectModel *self) {
  if (self == NULL)
    return;
  free(self->vertices);
  free(self);
}
