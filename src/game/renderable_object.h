#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "game/game_world.h"
#include "util/mat4.h"

typedef struct {
    int asset_id;
    Mat4 model_matrix;
} RenderableObject;

void renderable_object_render(RenderableObject self, GameWorld *world);

#endif
