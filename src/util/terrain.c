#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include "terrain.h"
#include "simplex_noise.h"
#include "random.h"
#include "util/array_list.h"

#define TERRAIN_SIZE 64
#define OCTAVES 5
#define JAGGEDNESS 1.0
#define DAMPENING 0.2

static float height_map[TERRAIN_SIZE][TERRAIN_SIZE];

static Vec3 start = { .data = { -32, 0, -32 } };
static Vec3 dimensions = { .data = { 32, 8, 32 } };
static Vec3 block_dimensions = { .data = { 20, 20, 20 } };

typedef struct {
    Vec3 a, b, c;
} Triangle;

static Vec3 triangle_normal(Triangle t){
    Vec3 e1 = vec3_sub(t.b, t.a);
    Vec3 e2 = vec3_sub(t.c, t.a);
    Vec3 n = vec3_cross(e2, e1);
    vec3_normalize(&n);
    return n; 
}

static void triangle_unload(Triangle t, float *arr){
    arr[0] = t.a.x;
    arr[1] = t.a.y;
    arr[2] = t.a.z;
    
    arr[3] = t.b.x;
    arr[4] = t.b.y;
    arr[5] = t.b.z;

    arr[6] = t.c.x;
    arr[7] = t.c.y;
    arr[8] = t.c.z;
}

static void normal_unload(Vec3 n, float *arr){
    arr[0] = n.x;
    arr[1] = n.y;
    arr[2] = n.z;
    
    arr[3] = n.x;
    arr[4] = n.y;
    arr[5] = n.z;

    arr[6] = n.x;
    arr[7] = n.y;
    arr[8] = n.z;
}

//todo:
//to optimize:
//-render quads instead of triangles
//-since there are only 5 possible normals (since its a cube with no rotation),
// instead of passing a normal, pass an index (0-4) into a normal array in the shader

Terrain terrain_new(){
    Terrain self;

    //2 triangles per point
    //3 vertices per triangle
    //3 floats per vertex
	self.num_floats = 0;
	self.vertices = NULL;
	self.normals = NULL;

	terrain_regen(&self);

    return self;
}

void terrain_regen(Terrain *self){
	free(self->vertices);
	free(self->normals);

	self->num_floats = 2 * 3 * 3 * (TERRAIN_SIZE - 1) * (TERRAIN_SIZE - 1);
	self->vertices = malloc(self->num_floats * sizeof(float));
	self->normals = malloc(self->num_floats * sizeof(float));

	int i, j, k;
	float tx, tz;
	float octave_vals[OCTAVES];
	float octave_ivals[OCTAVES];
	float inv_damp = 1.0 / DAMPENING;

	for (i = 0; i < OCTAVES; i++){
		octave_vals[i] = pow(inv_damp, i);
		octave_ivals[i] = 1.0 / octave_vals[i];
	}

	float sx = random_in_rangef(0.0f, 100.0f);
	float sy = random_in_rangef(0.0f, 100.0f);

	for (i = 0; i < TERRAIN_SIZE; i++){
		for (j = 0; j < TERRAIN_SIZE; j++){
			tx = JAGGEDNESS * i / TERRAIN_SIZE;
			tz = JAGGEDNESS * j / TERRAIN_SIZE;

			height_map[i][j] = 0;

			for (k = 0; k < OCTAVES; k++){
				height_map[i][j] += octave_ivals[k] * simplex_noise(sx + octave_vals[k] * tx, sy + octave_vals[k] * tz);
			}

			height_map[i][j] *= dimensions.y;
			height_map[i][j] = block_dimensions.y * ((int)(height_map[i][j]) + start.y);
		}
	}

    ArrayList_f *extra_vertices = array_list_new_f();
    ArrayList_f *extra_normals = array_list_new_f();

	int ind;
	Vec3 a, b, e1, e2, normal;
	Triangle t;
	float ti[3], tj[3];
	for (i = 0; i < TERRAIN_SIZE - 1; i++){
		for (j = 0; j < TERRAIN_SIZE - 1; j++){
			ind = 18 * (i + j * (TERRAIN_SIZE - 1));
			ti[1] = (start.x + i) * block_dimensions.x;
			ti[0] = ti[1] - block_dimensions.x;
			ti[2] = ti[1] + block_dimensions.x;
			tj[1] = (start.z + j) * block_dimensions.z;
			tj[2] = tj[1] - block_dimensions.z;
			tj[2] = tj[1] + block_dimensions.z;

			//triangle 1
            t.a = vec3_from_3f(ti[1], height_map[i][j], tj[1]);
			t.b = vec3_from_3f(ti[2], height_map[i][j], tj[1]);
			t.c = vec3_from_3f(ti[1], height_map[i][j], tj[2]);
			normal = triangle_normal(t);
            normal_unload(normal, self->normals + ind);			
			triangle_unload(t, self->vertices + ind);

			//triangle 2
            t.a = vec3_from_3f(ti[1], height_map[i][j], tj[2]);
			t.b = vec3_from_3f(ti[2], height_map[i][j], tj[1]);
			t.c = vec3_from_3f(ti[2], height_map[i][j], tj[2]);
			normal = triangle_normal(t);
			normal_unload(normal, self->normals + ind + 9);
			triangle_unload(t, self->vertices + ind + 9);

            if(i > 0){
                float h[2];
                if(height_map[i-1][j] < height_map[i][j]){
                    //current place is higher than place to the left
                    //triangle 1
                    h[0] = height_map[i-1][j];
                    h[1] = height_map[i][j];
                    t.a = vec3_from_3f(ti[1], h[0], tj[1]);
                    t.b = vec3_from_3f(ti[1], h[1], tj[1]);
                    t.c = vec3_from_3f(ti[1], h[0], tj[2]);
                    normal = triangle_normal(t);
                    if(extra_normals->capacity - extra_normals->length <= 9) array_list_grow_f(extra_normals);
                    normal_unload(normal, extra_normals->data + extra_normals->length);
                    extra_normals->length += 9;
                    if(extra_vertices->capacity - extra_vertices->length <= 9) array_list_grow_f(extra_vertices);
                    triangle_unload(t, extra_vertices->data + extra_vertices->length);
                    extra_vertices->length += 9;

                    //triangle 2
                    t.a = vec3_from_3f(ti[1], h[0], tj[2]);
                    t.b = vec3_from_3f(ti[1], h[1], tj[1]);
                    t.c = vec3_from_3f(ti[1], h[1], tj[2]);
                    normal = triangle_normal(t);
                    if(extra_normals->capacity - extra_normals->length <= 9) array_list_grow_f(extra_normals);
                    normal_unload(normal, extra_normals->data + extra_normals->length);
                    extra_normals->length += 9;
                    if(extra_vertices->capacity - extra_vertices->length <= 9) array_list_grow_f(extra_vertices);
                    triangle_unload(t, extra_vertices->data + extra_vertices->length);
                    extra_vertices->length += 9;
                }else if(height_map[i-1][j] > height_map[i][j]){
                    //current place is lower than the place to the left
					//triangle 1
					h[0] = height_map[i][j];
					h[1] = height_map[i-1][j];
					t.a = vec3_from_3f(ti[1], h[0], tj[1]);
					t.b = vec3_from_3f(ti[1], h[1], tj[1]);
					t.c = vec3_from_3f(ti[1], h[0], tj[2]);
					normal = triangle_normal(t);
					if(extra_normals->capacity - extra_normals->length <= 9) array_list_grow_f(extra_normals);
					normal_unload(normal, extra_normals->data + extra_normals->length);
					extra_normals->length += 9;
					if(extra_vertices->capacity - extra_vertices->length <= 9) array_list_grow_f(extra_vertices);
					triangle_unload(t, extra_vertices->data + extra_vertices->length);
					extra_vertices->length += 9;

					//triangle 2
					t.a = vec3_from_3f(ti[1], h[0], tj[2]);
					t.b = vec3_from_3f(ti[1], h[1], tj[1]);
					t.c = vec3_from_3f(ti[1], h[1], tj[2]);
					normal = triangle_normal(t);
					if(extra_normals->capacity - extra_normals->length <= 9) array_list_grow_f(extra_normals);
					normal_unload(normal, extra_normals->data + extra_normals->length);
					extra_normals->length += 9;
					if(extra_vertices->capacity - extra_vertices->length <= 9) array_list_grow_f(extra_vertices);
					triangle_unload(t, extra_vertices->data + extra_vertices->length);
					extra_vertices->length += 9;
                }
            }

            if(j > 0){
				float h[2];
                if(height_map[i][j-1] < height_map[i][j]){
                    //current place is higher than the place to the bottom
					//triangle 1
					h[0] = height_map[i][j-1];
					h[1] = height_map[i][j];
					t.a = vec3_from_3f(ti[1], h[0], tj[1]);
					t.b = vec3_from_3f(ti[1], h[1], tj[1]);
					t.c = vec3_from_3f(ti[2], h[0], tj[1]);
					normal = triangle_normal(t);
					if (extra_normals->capacity - extra_normals->length <= 9) array_list_grow_f(extra_normals);
					normal_unload(normal, extra_normals->data + extra_normals->length);
					extra_normals->length += 9;
					if (extra_vertices->capacity - extra_vertices->length <= 9) array_list_grow_f(extra_vertices);
					triangle_unload(t, extra_vertices->data + extra_vertices->length);
					extra_vertices->length += 9;

					//triangle 2
					t.a = vec3_from_3f(ti[2], h[0], tj[1]);
					t.b = vec3_from_3f(ti[1], h[1], tj[1]);
					t.c = vec3_from_3f(ti[2], h[1], tj[1]);
					normal = triangle_normal(t);
					if (extra_normals->capacity - extra_normals->length <= 9) array_list_grow_f(extra_normals);
					normal_unload(normal, extra_normals->data + extra_normals->length);
					extra_normals->length += 9;
					if (extra_vertices->capacity - extra_vertices->length <= 9) array_list_grow_f(extra_vertices);
					triangle_unload(t, extra_vertices->data + extra_vertices->length);
					extra_vertices->length += 9;
                }else if(height_map[i][j-1] > height_map[i][j]){
                    //current place is lower than the place to the bottom
					h[0] = height_map[i][j];
					h[1] = height_map[i][j-1];
					t.a = vec3_from_3f(ti[1], h[0], tj[1]);
					t.b = vec3_from_3f(ti[1], h[1], tj[1]);
					t.c = vec3_from_3f(ti[2], h[0], tj[1]);
					normal = triangle_normal(t);
					if(extra_normals->capacity - extra_normals->length <= 9) array_list_grow_f(extra_normals);
					normal_unload(normal, extra_normals->data + extra_normals->length);
					extra_normals->length += 9;
					if(extra_vertices->capacity - extra_vertices->length <= 9) array_list_grow_f(extra_vertices);
					triangle_unload(t, extra_vertices->data + extra_vertices->length);
					extra_vertices->length += 9;

					//triangle 2
					t.a = vec3_from_3f(ti[2], h[0], tj[1]);
					t.b = vec3_from_3f(ti[1], h[1], tj[1]);
					t.c = vec3_from_3f(ti[2], h[1], tj[1]);
					normal = triangle_normal(t);
					if(extra_normals->capacity - extra_normals->length <= 9) array_list_grow_f(extra_normals);
					normal_unload(normal, extra_normals->data + extra_normals->length);
					extra_normals->length += 9;
					if(extra_vertices->capacity - extra_vertices->length <= 9) array_list_grow_f(extra_vertices);
					triangle_unload(t, extra_vertices->data + extra_vertices->length);
					extra_vertices->length += 9;
                }
            }
		}
	}
    int new_length = self->num_floats + extra_vertices->length;
    self->vertices = realloc(self->vertices, new_length * sizeof(float));
    self->normals = realloc(self->normals, new_length * sizeof(float));
    memcpy(self->vertices + self->num_floats, extra_vertices->data, extra_vertices->length * sizeof(float));
    memcpy(self->normals + self->num_floats, extra_normals->data, extra_normals->length * sizeof(float));
    self->num_floats = new_length;
    array_list_free_f(extra_vertices);
    array_list_free_f(extra_normals);
}

void terrain_free(Terrain self){
    free(self.vertices);
	free(self.normals);
}
