#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "util/array_list.h"
#include "util/set.h"
#include "util/object_model.h"
#include "util/shader.h"
#include "util/obb.h"
#include "util/rect.h"
#include "util/mat4.h"

#include "renderer.h"

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


    Renderer *renderer;

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
=======
void game_world_render(GameWorld *self, Mat4 projection_matrix, Mat4 view_matrix);
>>>>>>> 702ba60... Moving rendering into the renderer object... untested but builds

int game_world_get_model_id(GameWorld *self, const char *name);
Obb game_world_get_model_obb(GameWorld *self, int asset_id);

Vec3 game_world_world_coords_to_screen_coords(GameWorld *self, Vec3 world_coords);

void game_world_apply_to_enemies(GameWorld *self, Vec3 position, float radius, void (*fn)(GameWorld *self, Enemy *enemy));
int game_world_is_colliding_with_wall(GameWorld *self, CollidableObject collidable);

#endif
