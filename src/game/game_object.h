#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "game_world.h"

typedef enum {
    GAME_OBJECT_TYPE_PLAYER,
    GAME_OBJECT_TYPE_ENEMY,
    GAME_OBJECT_TYPE_SPELL,
    GAME_OBJECT_TYPE_WALL,
} GameObjectType;

typedef struct {
    //type so we can pass around GameObject * but know what it needs to be
    //cast as
    GameObjectType type;

    //a reference to the gameworld it is currently in
    GameWorld *world;

    //set to true if this object should be destroyed
    int _destroy;
} GameObject;

GameObject *game_object_new(GameWorld *world, GameObjectType type);
void game_object_free(GameObject *self);

#endif
