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
    //type so we can pass around GameObject * but know what it needs to be
    //cast as
    GameObjectType type;

    //movement
    Vec3 position;
    Vec3 direction;

    //things you need to draw things
    int asset_id;
    Mat4 model_matrix;

    //bounding box for it
    Rect bounding_box;

    //a reference to the gameworld it is currently in
    GameWorld *world;

    //set to true if this object should be destroyed
    int _destroy;
} GameObject;

GameObject *game_object_new(GameWorld *world, GameObjectType type);
void game_object_free(GameObject *self);

void game_object_update(GameObject *self, double dt);
void game_object_render(GameObject *self);

#endif
