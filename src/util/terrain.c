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
#define JAGGEDNESS 0.5
#define DAMPENING 0.2

TerrainVertex TERRAIN_VERTEX_DEFAULT = { .position = { 0, 0, 0 }, .normal = { 0, 0, 0 }, .texture = { 0, 0 } };

/*
oct: 5
jagg: 1.0
damp: 0.2
91.235085 97.946098
5.478073 9.646901
*/

/*
oct: 5
jagg: 0.5
damp: 0.2
25.345623 65.031891
88.967560 49.69634
*/

static float height_map[TERRAIN_SIZE][TERRAIN_SIZE];

static float height = TERRAIN_SIZE * 0.25;
static Vec3 start = { .data = { -TERRAIN_SIZE * 0.5, -TERRAIN_SIZE * 0.125, -TERRAIN_SIZE * 0.5 } };
static Vec3 block_dimensions = { .data = { 5, 5, 5 } };

typedef struct {
    Vec3 p[4];
} Quad;

static Vec3 quad_normal(Quad q){
    Vec3 e1 = vec3_sub(q.p[1], q.p[0]);
    Vec3 e2 = vec3_sub(q.p[3], q.p[0]);
    Vec3 n = vec3_cross(e2, e1);
    vec3_normalize(&n);
    return n;
}

static void terrain_vertex_unload(TerrainVertex *arr, Quad q, Vec3 n){
    int i;
    for(i = 0;i < 4;i++){
        arr[i].position[0] = q.p[i].x;
        arr[i].position[1] = q.p[i].y;
        arr[i].position[2] = q.p[i].z;

        arr[i].normal[0] = n.x;
        arr[i].normal[1] = n.y;
        arr[i].normal[2] = n.z;
    }

    arr[0].texture[0] = 0;
    arr[0].texture[1] = 1;

    arr[1].texture[0] = 0;
    arr[1].texture[1] = 0;

    arr[2].texture[0] = 1;
    arr[2].texture[1] = 0;

    arr[3].texture[0] = 1;
    arr[3].texture[1] = 1;
}

static void terrain_vertex_push(ArrayList_tv *arr, Quad q, Vec3 n){
    TerrainVertex vs[4];
    int i;
    for(i = 0;i < 4;i++){
        vs[i].position[0] = q.p[i].x;
        vs[i].position[1] = q.p[i].y;
        vs[i].position[2] = q.p[i].z;

        vs[i].normal[0] = n.x;
        vs[i].normal[1] = n.y;
        vs[i].normal[2] = n.z;
    }

    vs[0].texture[0] = 0;
    vs[0].texture[1] = 1;

    vs[1].texture[0] = 0;
    vs[1].texture[1] = 0;

    vs[2].texture[0] = 1;
    vs[2].texture[1] = 0;

    vs[3].texture[0] = 1;
    vs[3].texture[1] = 1;

    array_list_push_tv(arr, vs[0]);
    array_list_push_tv(arr, vs[1]);
    array_list_push_tv(arr, vs[2]);
    array_list_push_tv(arr, vs[3]);
}


//todo:
//to optimize:
//-since there are only 5 possible normals (since its a cube with no rotation),
// instead of passing a normal, pass an index (0-4) into a normal array in the shader

Terrain terrain_new(){
    Terrain self;

    self.num_grass_vertices = 0;
    self.grass_vertices = NULL;

    self.num_grass_dirt_vertices = 0;
    self.grass_dirt_vertices = NULL;

    self.num_dirt_vertices = 0;
    self.dirt_vertices = NULL;

	terrain_regen(&self);

    return self;
}

void terrain_regen(Terrain *self){
    free(self->grass_vertices);
    free(self->grass_dirt_vertices);
    free(self->dirt_vertices);

    self->num_grass_vertices = 4 * TERRAIN_SIZE * TERRAIN_SIZE;
    self->grass_vertices = malloc(self->num_grass_vertices * sizeof(*(self->grass_vertices)));

	int i, j, k;
	float tx, tz;
	float octave_vals[OCTAVES];
	float octave_ivals[OCTAVES];
	float inv_damp = 1.0 / DAMPENING;

	for (i = 0; i < OCTAVES; i++){
		octave_vals[i] = pow(inv_damp, i);
		octave_ivals[i] = 1.0 / octave_vals[i];
	}

    //pick random start position
	// float sx = random_in_rangef(0.0f, 100.0f);
	// float sy = random_in_rangef(0.0f, 100.0f);
    // printf("%f %f\n", sx, sy);
    float sx = 25.345623, sy = 65.031891;

    //compute the height for the terrain
	for (i = 0; i < TERRAIN_SIZE; i++){
		for (j = 0; j < TERRAIN_SIZE; j++){
			tx = JAGGEDNESS * i / TERRAIN_SIZE;
			tz = JAGGEDNESS * j / TERRAIN_SIZE;

			height_map[i][j] = 0;

			for (k = 0; k < OCTAVES; k++){
				height_map[i][j] += octave_ivals[k] * simplex_noise(sx + octave_vals[k] * tx, sy + octave_vals[k] * tz);
			}

			height_map[i][j] *= height;
			height_map[i][j] = block_dimensions.y * ((int)(height_map[i][j]) + start.y);
		}
	}

    //build the mesh for the terrain
    ArrayList_tv *extra_vertices = array_list_new_tv();

	int ind;
	Vec3 normal;
    Quad q;
    TerrainVertex v;
	float ti[2], tj[2];
	for (i = 0; i < TERRAIN_SIZE; i++){
		for (j = 0; j < TERRAIN_SIZE; j++){
			ind = 4 * (i + j * TERRAIN_SIZE);

            //x positions
            ti[0] = (start.x + i) * block_dimensions.x;
            ti[1] = ti[0] + block_dimensions.x;

            //z positions
			tj[0] = (start.z + j) * block_dimensions.z;
			tj[1] = tj[0] + block_dimensions.z;

            q.p[0] = vec3_from_3f(ti[0], height_map[i][j], tj[0]);
            q.p[1] = vec3_from_3f(ti[1], height_map[i][j], tj[0]);
            q.p[2] = vec3_from_3f(ti[1], height_map[i][j], tj[1]);
            q.p[3] = vec3_from_3f(ti[0], height_map[i][j], tj[1]);
            normal = quad_normal(q);
            terrain_vertex_unload(self->grass_vertices + ind, q, normal);
            
            if(i < TERRAIN_SIZE - 1){
                float h[2];
                if(height_map[i][j] < height_map[i+1][j]){
                    //at start you are facing -z
                    //with +x to the right

                    //current spot is lower than place to the right
                    h[0] = height_map[i][j];
                    h[1] = height_map[i+1][j];

                    q.p[0] = vec3_from_3f(ti[1], h[0], tj[0]);
                    q.p[1] = vec3_from_3f(ti[1], h[1], tj[0]);
                    q.p[2] = vec3_from_3f(ti[1], h[1], tj[1]);
                    q.p[3] = vec3_from_3f(ti[1], h[0], tj[1]);
                    normal = quad_normal(q);
                    terrain_vertex_push(extra_vertices, q, normal);
                }else if(height_map[i][j] > height_map[i+1][j]){
                    //current spot is higher than place to the right
                    h[0] = height_map[i+1][j];
                    h[1] = height_map[i][j];

                    q.p[0] = vec3_from_3f(ti[1], h[0], tj[1]);
                    q.p[1] = vec3_from_3f(ti[1], h[1], tj[1]);
                    q.p[2] = vec3_from_3f(ti[1], h[1], tj[0]);
                    q.p[3] = vec3_from_3f(ti[1], h[0], tj[0]);
                    normal = quad_normal(q);
                    terrain_vertex_push(extra_vertices, q, normal);
                }
            }

            if(j < TERRAIN_SIZE - 1){
                float h[2];
                if(height_map[i][j] < height_map[i][j+1]){
                    //at start you are facing -z
                    //with +x to the right

                    //current spot is lower than place behind
                    h[0] = height_map[i][j];
                    h[1] = height_map[i][j+1];

                    q.p[0] = vec3_from_3f(ti[1], h[0], tj[1]);
                    q.p[1] = vec3_from_3f(ti[1], h[1], tj[1]);
                    q.p[2] = vec3_from_3f(ti[0], h[1], tj[1]);
                    q.p[3] = vec3_from_3f(ti[0], h[0], tj[1]);
                    normal = quad_normal(q);
                    terrain_vertex_push(extra_vertices, q, normal);                    
                }else if(height_map[i][j] > height_map[i][j+1]){
                    //current spot is higher than place behind
                    h[0] = height_map[i][j+1];
                    h[1] = height_map[i][j];

                    q.p[0] = vec3_from_3f(ti[0], h[0], tj[1]);
                    q.p[1] = vec3_from_3f(ti[0], h[1], tj[1]);
                    q.p[2] = vec3_from_3f(ti[1], h[1], tj[1]);
                    q.p[3] = vec3_from_3f(ti[1], h[0], tj[1]);
                    normal = quad_normal(q);
                    terrain_vertex_push(extra_vertices, q, normal);                    
                }
            }
		}
	}

    self->num_grass_dirt_vertices = extra_vertices->length;
    self->grass_dirt_vertices = extra_vertices->data;
    extra_vertices->data = NULL;
    array_list_free_tv(extra_vertices);    
}

void terrain_free(Terrain self){
	free(self.grass_vertices);
	free(self.grass_dirt_vertices);
	free(self.dirt_vertices);
}
