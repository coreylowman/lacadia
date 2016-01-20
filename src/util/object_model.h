#ifndef OBJ_MODEL_H
#define OBJ_MODEL_H

#include <stdlib.h>

#include "obb.h"

typedef struct {    
    size_t num_floats;
    float *vertices;
    float *colors;
    Obb bounding_box;
} ObjectModel;

ObjectModel *obj_model_from_file(const char *filename);

ObjectModel *obj_model_clone(ObjectModel *self);

void obj_model_free(ObjectModel *self);

#endif
