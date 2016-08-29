#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "util/vec3.h"
#include "util/renderer.h"

typedef void (*GameObjectUpdateCallback)(struct GameObject *self, double dt);
typedef void (*GameObjectRenderCallback)(struct GameObject *self, Renderer *renderer);
typedef void (*GameObjectFreeCallback)(struct GameObject *self);

typedef struct GameWorld GameWorld;
typedef struct Component Component;

typedef enum GameObjectType {
    GAME_OBJECT_TYPE_PLAYER,
    GAME_OBJECT_TYPE_ENEMY,
    GAME_OBJECT_TYPE_SPELL,
    GAME_OBJECT_TYPE_WALL,
    GAME_OBJECT_TYPE_PARTICLE_SYSTEM,
    GAME_OBJECT_TYPE_MAX,
} GameObjectType;

typedef struct GameObject {
    //type so we can pass around GameObject * but know what it needs to be
    //cast as
    GameObjectType type;
    
    //a reference to the gameworld it is currently in
    GameWorld *world;

	// all game objects have a position and direction that they are facing
	Vec3 position;
	Vec3 direction;

	// callbacks to update and render passed into the init function.
    GameObjectUpdateCallback on_update;
    GameObjectRenderCallback on_render;
    GameObjectFreeCallback on_free;

    //set to true if this object should be destroyed
    int destroy;
} GameObject;

GameObject game_object_init(GameWorld *world, GameObjectType type, GameObjectUpdateCallback on_update, GameObjectRenderCallback on_render, GameObjectFreeCallback on_free);
float game_object_get_y_rotation(GameObject *self);

void game_object_move_by(GameObject *self, Vec3 velocity);
void game_object_move(GameObject *self, double scalar);

void game_object_update(GameObject *self, double dt);
void game_object_render(GameObject *self, Renderer *renderer);
void game_object_free(GameObject *self);

#endif
