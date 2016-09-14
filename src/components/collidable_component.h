#ifndef COLLIDABLE_COMPONENT_H
#define COLLIDABLE_COMPONENT_H

#include "component.h"
#include "util/obb.h"
#include "game/game_object.h"

struct CollidableComponent;

typedef void (*OnCollideCallback)(GameObject *self, GameObject *other);
typedef int (*IsCollidingCallback)(struct CollidableComponent self, struct CollidableComponent other);

struct CollidableComponent {
    Component base_component;

    Obb bounding_box;

    IsCollidingCallback is_colliding;
    OnCollideCallback on_collide;
};

typedef struct CollidableComponent CollidableComponent;

CollidableComponent collidable_component_init(GameObject *container, Obb bounding_box, OnCollideCallback on_collide);
int collidable_component_is_colliding(CollidableComponent self, CollidableComponent other);
void collidable_component_set_scale(CollidableComponent *self, float scale);

#endif
