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

	self->terrain = terrain_new();

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

void level_render(Level *self, Renderer *renderer){
    int i;
    for(i = 0;i < self->num_walls;i++){
        wall_render(self->walls[i], renderer);
    }
    renderer_render_terrain(renderer, self->terrain);
	// int num_triangles = self->terrain.num_floats / 3;
	// int ind;
	// Vec3 a, b, n;
	// Line l; 
	// int start = 30000;
	// int end = start + 1024;
	// for (i = start; i < end; i+=3){
	// 	ind = 3 * i;
	// 	a = vec3_from_3f(self->terrain.vertices[ind], self->terrain.vertices[ind + 1], self->terrain.vertices[ind + 2]);
	// 	a = vec3_add(a, vec3_from_3f(self->terrain.vertices[ind + 3], self->terrain.vertices[ind + 4], self->terrain.vertices[ind + 5]));
	// 	a = vec3_add(a, vec3_from_3f(self->terrain.vertices[ind + 6], self->terrain.vertices[ind + 7], self->terrain.vertices[ind + 8]));
	// 	a = vec3_scale(a, 0.333333333333f);
	// 	n = vec3_from_3f(self->terrain.normals[ind], self->terrain.normals[ind + 1], self->terrain.normals[ind + 2]);
	// 	b = vec3_add(a, n);
	// 	l.start = a;
	// 	l.end = b;
	// 	renderer_render_line(renderer, l);
	// }
}
