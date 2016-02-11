#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "game_world.h"
#include "util/mat4.h"
#include "moveable_object.h"

typedef struct {
    int asset_id;
    Mat4 model_matrix;
} RenderableObject;

RenderableObject renderable_object_init(const char *asset_name);

void renderable_object_update(RenderableObject *self, MoveableObject moveable);

void renderable_object_render(RenderableObject self, GameWorld *world);

#endif
