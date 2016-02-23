#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "util/renderer.h"
#include "util/mat4.h"
#include "moveable_object.h"

typedef struct {
    int model_id;
    Mat4 model_matrix;
} RenderableObject;

RenderableObject renderable_object_init(const char *model_name, Renderer *renderer);
void renderable_object_set_model_matrix(RenderableObject *self, Mat4 model_matrix);
void renderable_object_update(RenderableObject *self, MoveableObject moveable);
void renderable_object_render(RenderableObject self, Renderer *renderer);

#endif
