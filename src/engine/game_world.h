#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "engine/util/array_list.h"
#include "engine/util/sparse_array.h"
#include "engine/util/object_model.h"
#include "engine/util/obb.h"
#include "engine/util/rect.h"
#include "engine/util/mat4.h"
#include "engine/util/camera.h"
#include "engine/util/inputs.h"
#include "engine/graphics/renderer.h"
#include "asset_manager.h"

typedef struct Player Player;
typedef struct Enemy Enemy;
typedef struct Level Level;
typedef struct Wall Wall;
typedef struct CollidableComponent CollidableComponent;
typedef struct GameObject GameObject;
typedef enum GameObjectType GameObjectType;

typedef struct GameWorld {
  double dt;

  SparseArray *game_objects; // SparseArray<GameObject *>

  AssetManager *asset_manager;
  Renderer *renderer;

  Terrain terrain;

  // matrix for converting world to screen
  // this is just projection * view
  Mat4 world_to_screen;
  Mat4 screen_to_world;

  Camera camera;
  Inputs inputs;
} GameWorld;

GameWorld *game_world_new();
void game_world_free(GameWorld *self);

GameObject *game_world_get_first_tagged(GameWorld *self, const char *tag);

void game_world_add_object(GameWorld *self, GameObject *object);

void game_world_update(GameWorld *self, double dt);

int game_world_get_asset_id(GameWorld *self, const char *name);
Obb game_world_get_asset_obb(GameWorld *self, int asset_id);

void game_world_render(GameWorld *self);

int game_world_get_model_id(GameWorld *self, const char *name);
Obb game_world_get_model_obb(GameWorld *self, int asset_id);

Vec3 game_world_world_to_screen(GameWorld *self, Vec3 world_coords);
Vec3 game_world_screen_to_world(GameWorld *self, float mousex, float mousey);

void game_world_apply(GameWorld *self, const char *tag, GameObject *user,
                      float radius,
                      void (*fn)(GameWorld *world, GameObject *obj,
                                 GameObject *target));

#endif
