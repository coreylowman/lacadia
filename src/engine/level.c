#include <stdlib.h>
#include "level.h"
#include "engine/util/vec3.h"
#include "engine/util/mat4.h"
#include "engine/util/terrain.h"

void terrain_vert_callback(TerrainVertex *vert) {

}

Level *level_new(GameWorld *world) {
  Level *self = malloc(sizeof(*self));

  self->world = world;

  Vec3 dims = wall_dimensions(world);

  int num_walls = 4;
  Wall *walls[4];
  walls[0] = wall_new(world, (Vec3){.data = {dims.x * -5, 0.0, dims.z * -5}},
                      VEC3_UNIT_X, 10);
  walls[1] = wall_new(world, (Vec3){.data = {dims.x * 5, 0.0, dims.z * -5}},
                      VEC3_UNIT_Z, 10);
  walls[2] = wall_new(world, (Vec3){.data = {dims.x * -4, 0.0, dims.z * 5}},
                      VEC3_UNIT_X, 10);
  walls[3] = wall_new(world, (Vec3){.data = {dims.x * -5, 0.0, dims.z * -4}},
                      VEC3_UNIT_Z, 10);

  int i;
  for (i = 0; i < num_walls; i++) {
    game_world_add_object(world, (GameObject *)walls[i]);
  }

  self->terrain = terrain_new(terrain_vert_callback, 100, 50, 100, 5);

  return self;
}

void level_free(Level *self) {
  self->world = NULL;
  terrain_free(self->terrain);
  free(self);
}

void level_render(Level *self, Renderer *renderer) {
  renderer_render_terrain(renderer, self->terrain);
  // int num_quads = self->terrain.num_floats / 12;
  // int ind;
  // Vec3 a, b, n;
  // Line l;
  // for (i = 0; i < num_quads; i++){
  // 	ind = 12 * i;
  // 	a = vec3_from_3f(self->terrain.vertices[ind], self->terrain.vertices[ind
  // + 1], self->terrain.vertices[ind + 2]);
  // 	a = vec3_add(a, vec3_from_3f(self->terrain.vertices[ind + 3],
  // self->terrain.vertices[ind + 4], self->terrain.vertices[ind + 5]));
  //        a = vec3_add(a, vec3_from_3f(self->terrain.vertices[ind + 6],
  //        self->terrain.vertices[ind + 7], self->terrain.vertices[ind + 8]));
  // 	a = vec3_add(a, vec3_from_3f(self->terrain.vertices[ind + 9],
  // self->terrain.vertices[ind + 10], self->terrain.vertices[ind + 11]));
  // 	a = vec3_scale(a, 0.25);
  // 	n = vec3_from_3f(self->terrain.normals[ind], self->terrain.normals[ind +
  // 1], self->terrain.normals[ind + 2]);
  // 	b = vec3_add(a, n);
  // 	l.start = a;
  // 	l.end = b;
  // 	renderer_render_line(renderer, l);
  // }
}
