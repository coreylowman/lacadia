#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "shaders.h"
#include "array_list.h"
#include "object_model.h"

#define MAX_ASSETS 10

typedef struct {
    void *player;
    ArrayList *spells; //ArrayList<Spell *>
    ArrayList *enemies; //ArrayList<Enemy *>

    int num_assets;
    char *asset_names[MAX_ASSETS];
    ObjectModel *asset_models[MAX_ASSETS];
	ArrayList_m4 *asset_model_matrices[MAX_ASSETS]; //ArrayList<Mat4>[MAX_ASSETS]

    unsigned int vbo[3], vao;
} GameWorld;

GameWorld *game_world_new();
void game_world_free(GameWorld *self);

void game_world_update(GameWorld *self, double dt);
void game_world_render(GameWorld *self, Shader shader);

int game_world_get_asset_id(GameWorld *self, const char *name);

//use to copy verts & colors into vertices/colors array list
//will then draw the whole thing in render
void game_world_draw_asset(GameWorld *self, int asset_id, Mat4 model_matrix);

#endif
