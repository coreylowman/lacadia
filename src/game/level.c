#include <stdlib.h>
#include "level.h"
#include "util/vec3.h"

extern Vec3 VEC3_UNIT_X;
extern Vec3 VEC3_UNIT_Z;

Level *level_new(GameWorld *world){
    Level *self = malloc(sizeof(*self));

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
    return self;
}

void level_free(Level *self){
    int i;
    for(i = 0;i < self->num_walls;i++){
        wall_free(self->walls[i]);
    }
    free(self);
}

void level_render(Level *self){
    int i;
    for(i = 0;i < self->num_walls;i++){
        wall_render(self->walls[i]);
    }
}
