#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "util/array_list.h"
#include "util/set.h"
#include "util/object_model.h"
#include "util/shaders.h"

#define MAX_ASSETS 20

typedef struct GameWorld {
    void *player;
    Set *spells; //Set<Spell *>
    Set *enemies; //Set<Enemy *>

    int num_assets;
    char *asset_names[MAX_ASSETS];
    ObjectModel *asset_models[MAX_ASSETS];
	ArrayList_m4 *asset_model_matrices[MAX_ASSETS]; //ArrayList<Mat4>[MAX_ASSETS]

    unsigned int vbo[3], vao;
} GameWorld;

GameWorld *game_world_new();
void game_world_free(GameWorld *self);

void game_world_add_spell(GameWorld *self, void *s);
void game_world_add_enemy(GameWorld *self, void *e);

void game_world_update(GameWorld *self, double dt);
void game_world_render(GameWorld *self, Shader shader);

int game_world_get_asset_id(GameWorld *self, const char *name);
Rect game_world_get_asset_aabb(GameWorld *self, int asset_id);

//use to copy verts & colors into vertices/colors array list
//will then draw the whole thing in render
void game_world_draw_asset(GameWorld *self, int asset_id, Mat4 model_matrix);

#endif
