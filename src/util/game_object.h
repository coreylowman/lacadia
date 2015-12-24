#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "game_world.h"
#include "rect.h"
#include "vec3.h"
#include "mat4.h"

typedef enum {
    GAME_OBJECT_TYPE_PLAYER,
    GAME_OBJECT_TYPE_ENEMY,
    GAME_OBJECT_TYPE_SPELL,
} GameObjectType;

typedef struct {
    GameObjectType type;

    Vec3 position;
    Vec3 direction;
    Rect bounding_box;
    
    GameWorld *world;

    //set to true if this object should be destroyed
    int destroy;
} GameObject;

GameObject *game_object_new(GameWorld *world, GameObjectType type);
void game_object_free(GameObject *self);

void game_object_update(GameObject *self, double dt);

#endif
