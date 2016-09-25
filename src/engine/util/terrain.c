#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <GL/glew.h>
#include "terrain.h"
#include "simplex_noise.h"
#include "random.h"
#include "engine/util/array_list.h"

#define OCTAVES 5
#define JAGGEDNESS 1.5
#define DETAIL 2.0
#define FLATTENING 2.5
#define RANDOM_TERRAIN

static float sx =81.270790, sy = 43.907982;

typedef struct { Vec3 p[3]; } Tri;

static Vec3 tri_normal(Tri q) {
  Vec3 e1 = vec3_sub(q.p[1], q.p[0]);
  Vec3 e2 = vec3_sub(q.p[2], q.p[0]);
  Vec3 n = vec3_cross(e2, e1);
  vec3_normalize(&n);
  return n;
}

static void terrain_vertex_unload(TerrainVertex *arr, Tri q, Vec3 n,
                                  Vec3 color) {
  int i;
  for (i = 0; i < 3; i++) {
    arr[i].position[0] = q.p[i].x;
    arr[i].position[1] = q.p[i].y;
    arr[i].position[2] = q.p[i].z;

    arr[i].normal[0] = n.x;
    arr[i].normal[1] = n.y;
    arr[i].normal[2] = n.z;

    arr[i].color[0] = color.x;
    arr[i].color[1] = color.y;
    arr[i].color[2] = color.z;
  }
}

Terrain terrain_new(TerrainVertexCallback vertexCallback, int width, int height,
                    int length, int unit_size) {
  Terrain self;

  self.unit_size = unit_size;
  self.width = width;
  self.height = height;
  self.length = length;

  self.height_map = malloc(width * length * sizeof(float));

  self.num_vertices = 0;
  self.vertices = NULL;

  terrain_regen(&self, vertexCallback);

  return self;
}

#define VEC3(_x, _y, _z)                                                       \
  (Vec3) { .x = _x, .y = _y, .z = _z }

void terrain_regen(Terrain *self, TerrainVertexCallback vertexCallback) {
  free(self->vertices);

  self->num_vertices = 2 * 3 * self->width * self->length;
  self->vertices = malloc(self->num_vertices * sizeof(*(self->vertices)));

  int i, j, k;
  float tx, tz;
  float octave_vals[OCTAVES];
  float octave_ivals[OCTAVES];

  for (i = 0; i < OCTAVES; i++) {
    octave_vals[i] = pow(DETAIL, i);
    octave_ivals[i] = 1.0 / octave_vals[i];
  }

// pick random start position
#ifdef RANDOM_TERRAIN
  sx = random_in_rangef(0.0f, 100.0f);
  sy = random_in_rangef(0.0f, 100.0f);
  printf("%f %f\n", sx, sy);
#endif

  int ind;
  float max_height = 0;
  float min_height = FLT_MAX;
  // compute the height for the terrain
  for (i = 0; i < self->width; i++) {
    for (j = 0; j < self->length; j++) {
      ind = i + self->width * j;
      tx = (JAGGEDNESS * i) / self->width;
      tz = (JAGGEDNESS * j) / self->length;

      self->height_map[ind] = 0;

      for (k = 0; k < OCTAVES; k++) {
        self->height_map[ind] +=
            octave_ivals[k] *
            simplex_noise(sx + octave_vals[k] * tx, sy + octave_vals[k] * tz);
      }

      if (self->height_map[ind] > max_height) {
        max_height = self->height_map[ind];
      }

      if (self->height_map[ind] < min_height) {
        min_height = self->height_map[ind];
      }
    }
  }

  // normalize [min_height, max_height] to [0, height]
  float diff = max_height - min_height;
  for (i = 0; i < self->width; i++) {
    for (j = 0; j < self->length; j++) {
      ind = i + self->width * j;

      self->height_map[ind] -= min_height;
      self->height_map[ind] /= diff;

      self->height_map[ind] = powf(self->height_map[ind], FLATTENING);

      self->height_map[ind] *= self->height;
    }
  }

  // create mesh
  Vec3 brown = VEC3(156.0f / 255.0f, 143.0f / 255.0f, 124.0f / 255.0f);
  Vec3 normal;
  Tri q;
  float ti[2], tj[2];
  float h[4] = {0};
  for (i = 0; i < self->width; i++) {
    for (j = 0; j < self->length; j++) {
      ind = 2 * 3 * (i + j * self->width);

      // i,j
      h[0] = self->height_map[i + self->width * j];
      // i+1,j
      h[1] =
          i == self->width - 1 ? 0 : self->height_map[i + 1 + self->width * j];
      // i, j+1
      h[2] = j == self->length - 1
                 ? 0
                 : self->height_map[i + self->width * (j + 1)];
      // i+1,j+1
      if (i == self->width - 1 || j == self->length - 1) {
        h[3] = 0;
      } else {
        h[3] = self->height_map[i + 1 + self->width * (j + 1)];
      }

      // x positions
      ti[0] = (i - self->width * 0.5) * self->unit_size;
      ti[1] = ti[0] + self->unit_size;

      // z positions
      tj[0] = (j - self->length * 0.5) * self->unit_size;
      tj[1] = tj[0] + self->unit_size;

      q.p[0] = VEC3(ti[0], h[0], tj[0]);
      q.p[1] = VEC3(ti[1], h[1], tj[0]);
      q.p[2] = VEC3(ti[1], h[3], tj[1]);
      normal = tri_normal(q);
      terrain_vertex_unload(self->vertices + ind, q, normal, brown);

      q.p[0] = VEC3(ti[0], h[0], tj[0]);
      q.p[1] = VEC3(ti[1], h[3], tj[1]);
      q.p[2] = VEC3(ti[0], h[2], tj[1]);
      normal = tri_normal(q);
      terrain_vertex_unload(self->vertices + ind + 3, q, normal, brown);

      if (vertexCallback != NULL) {
        for (k = 0; k < 6; k++) {
          vertexCallback(self->vertices + ind + k);
        }
      }
    }
  }
}

void terrain_free(Terrain self) {
  free(self.height_map);
  free(self.vertices);
}
