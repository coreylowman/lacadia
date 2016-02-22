#include <stdlib.h>
#include "level.h"
#include "util/vec3.h"
#include "util/mat4.h"
#include "util/terrain.h"

extern Vec3 VEC3_UNIT_X;
extern Vec3 VEC3_UNIT_Z;
extern Vec3 VEC3_ZERO;
extern Mat4 MAT4_IDENT;

Level *level_new(GameWorld *world){
    Level *self = malloc(sizeof(*self));

    self->world = world;
    
    Vec3 dims = wall_dimensions(world);

    self->num_walls = 4;
    self->walls[0] = wall_new(world, (Vec3){ .data = { dims.x * -5, 0.0, dims.z * -5} }, VEC3_UNIT_X, 10);
	self->walls[1] = wall_new(world, (Vec3){ .data = { dims.x * 5, 0.0, dims.z * -5} }, VEC3_UNIT_Z, 10);
	self->walls[2] = wall_new(world, (Vec3){ .data = { dims.x * -4, 0.0, dims.z * 5} }, VEC3_UNIT_X, 10);
    self->walls[3] = wall_new(world, (Vec3){ .data = { dims.x * -5, 0.0, dims.z * -4} }, VEC3_UNIT_Z, 10);
    
    int i;
    for(i = 0;i < self->num_walls;i++){
        game_world_add_wall(world, self->walls[i], i);
    }

	self->terrain = terrain_new(vec3_from_3f(-16, 0, -16), vec3_from_3f(32, 32, 32));

    return self;
}

void level_free(Level *self){
    int i;
    self->world = NULL;
    for(i = 0;i < self->num_walls;i++){
        wall_free(self->walls[i]);
    }
    terrain_free(self->terrain);
    free(self);
}

void level_render(Level *self){
    int i;
    for(i = 0;i < self->num_walls;i++){
        wall_render(self->walls[i]);
    }
}

void level_render_terrain(Level *self){
    terrain_render(self->terrain);
}
