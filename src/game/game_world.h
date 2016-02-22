#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "util/array_list.h"
#include "util/set.h"
#include "util/object_model.h"
#include "util/shaders.h"
#include "util/obb.h"
#include "util/rect.h"
#include "util/mat4.h"

#define MAX_ASSETS 32
#define MAX_UI_RECTS 128

typedef struct Player Player;
typedef struct Enemy Enemy;
typedef struct Level Level;
typedef struct Wall Wall;
typedef struct CollidableObject CollidableObject;

typedef struct GameWorld {
    Player *player;
    Level *level;

    double dt;

    Set *spells; //Set<Spell *>
    Set *enemies; //Set<Enemy *>
    
    Set *collidables; //Set<CollidableObject *>
    //the ith element is the index in spells/enemies of the ith collidable
    Set *indices; //Set<int *>

    
    Set *particle_systems; //Set<ParticleSystem *>


    //models and stuff aka assets
    int num_assets;
    char *asset_names[MAX_ASSETS];
    ObjectModel *asset_models[MAX_ASSETS];
	ArrayList_m4 *asset_model_matrices[MAX_ASSETS]; //ArrayList<Mat4>[MAX_ASSETS]
    unsigned int asset_vbo[3], asset_vao;

    //ui rects and stuff
    int num_ui_rects;
    UIRect rects[MAX_UI_RECTS];
    Vec3 rect_colors[4 * MAX_UI_RECTS];
    unsigned int ui_vbo[2], ui_vao;


    //matrix for converting world to screen
    //this is just projection * view
    Mat4 world_to_screen;
} GameWorld;

GameWorld *game_world_new();
void game_world_free(GameWorld *self);

void game_world_set_player(GameWorld *self, Player *p);

void game_world_add_spell(GameWorld *self, void *s);
void game_world_add_enemy(GameWorld *self, void *e);
void game_world_add_wall(GameWorld *self, Wall *w, int index);
void game_world_add_particle_system(GameWorld *self, void *ps);

void game_world_update(GameWorld *self, double dt);
void game_world_render(GameWorld *self, Shader shader);
void game_world_render_ui(GameWorld *self, Shader shader);
void game_world_render_terrain(GameWorld *self, Shader shader);
void game_world_debug_render(GameWorld *self, Shader shader);

int game_world_get_asset_id(GameWorld *self, const char *name);
Obb game_world_get_asset_obb(GameWorld *self, int asset_id);

//use to copy verts & colors into vertices/colors array list
//will then draw the whole thing in render
void game_world_draw_asset(GameWorld *self, int asset_id, Mat4 model_matrix);

void game_world_draw_rect(GameWorld *self, Rect2 rect, Vec3 color);

Vec3 game_world_world_coords_to_screen_coords(GameWorld *self, Vec3 world_coords);

void game_world_apply_to_enemies(GameWorld *self, Vec3 position, float radius, void (*fn)(GameWorld *self, Enemy *enemy));
int game_world_is_colliding_with_wall(GameWorld *self, CollidableObject collidable);

#endif
