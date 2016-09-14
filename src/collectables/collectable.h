#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "game/game_world.h"
#include "game/game_object.h"
#include "components/collidable_component.h"
#include "components/renderable_component.h"
#include "util/renderer.h"

typedef struct Collectable {
    GameObject base_object;

    CollidableComponent collidable;
    RenderableComponent renderable;
} Collectable;

Collectable *collectable_new(GameWorld *world, GameObjectUpdateCallback on_update, GameObjectRenderCallback on_render, GameObjectFreeCallback on_free);
void collectable_free(GameObject *self);

void collectable_update(GameObject *self, double dt);
void collectable_render(GameObject *self, Renderer *renderer);

#endif
