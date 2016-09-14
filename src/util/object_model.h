#ifndef OBJ_MODEL_H
#define OBJ_MODEL_H

#include <stdlib.h>

#include "obb.h"

typedef struct {
  float position[3];
  float color[3];
  float normal[3];
} ObjectModelVertex;

typedef struct {
  // size_t num_floats;
  // float *vertices;
  // float *colors;
  int num_vertices;
  ObjectModelVertex *vertices;
  Obb bounding_box;
} ObjectModel;

ObjectModel *obj_model_from_file(const char *filename);

void obj_model_free(ObjectModel *self);

#endif
