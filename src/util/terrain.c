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

static void quad_unload(Quad t, float *arr){
    int i, ind;
    for(i = 0;i < 4;i++){
        ind = 3 * i;
        arr[ind + 0] = t.p[i].x;
        arr[ind + 1] = t.p[i].y;
        arr[ind + 2] = t.p[i].z;
    }
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

    arr[9] = n.x;
    arr[10] = n.y;
    arr[11] = n.z;
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

    //for each point in the terrain, there are 4 vertices, each with 3 floats
	self->num_floats = 4 * 3 * TERRAIN_SIZE * TERRAIN_SIZE;
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
    ArrayList_f *extra_vertices = array_list_new_f();
    ArrayList_f *extra_normals = array_list_new_f();

	int ind;
	Vec3 a, b, e1, e2, normal;
    Quad q;
	float ti[2], tj[2];
	for (i = 0; i < TERRAIN_SIZE; i++){
		for (j = 0; j < TERRAIN_SIZE; j++){
			ind = 12 * (i + j * TERRAIN_SIZE);
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
            normal_unload(normal, self->normals + ind);
            quad_unload(q, self->vertices + ind);

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

                    if(extra_normals->capacity - extra_normals->length <= 12) array_list_grow_f(extra_normals);
                    normal_unload(normal, extra_normals->data + extra_normals->length);
                    extra_normals->length += 12;

                    if(extra_vertices->capacity - extra_vertices->length <= 12) array_list_grow_f(extra_vertices);
                    quad_unload(q, extra_vertices->data + extra_vertices->length);
                    extra_vertices->length += 12;                    
                }else if(height_map[i][j] > height_map[i+1][j]){
                    //current spot is higher than place to the right
                    h[0] = height_map[i+1][j];
                    h[1] = height_map[i][j];

                    q.p[0] = vec3_from_3f(ti[1], h[0], tj[1]);
                    q.p[1] = vec3_from_3f(ti[1], h[1], tj[1]);
                    q.p[2] = vec3_from_3f(ti[1], h[1], tj[0]);
                    q.p[3] = vec3_from_3f(ti[1], h[0], tj[0]);
                    normal = quad_normal(q);

                    if(extra_normals->capacity - extra_normals->length <= 12) array_list_grow_f(extra_normals);
                    normal_unload(normal, extra_normals->data + extra_normals->length);
                    extra_normals->length += 12;

                    if(extra_vertices->capacity - extra_vertices->length <= 12) array_list_grow_f(extra_vertices);
                    quad_unload(q, extra_vertices->data + extra_vertices->length);
                    extra_vertices->length += 12;                    
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

                    if(extra_normals->capacity - extra_normals->length <= 12) array_list_grow_f(extra_normals);
                    normal_unload(normal, extra_normals->data + extra_normals->length);
                    extra_normals->length += 12;

                    if(extra_vertices->capacity - extra_vertices->length <= 12) array_list_grow_f(extra_vertices);
                    quad_unload(q, extra_vertices->data + extra_vertices->length);
                    extra_vertices->length += 12;                    
                }else if(height_map[i][j] > height_map[i][j+1]){
                    //current spot is higher than place behind
                    h[0] = height_map[i][j+1];
                    h[1] = height_map[i][j];

                    q.p[0] = vec3_from_3f(ti[0], h[0], tj[1]);
                    q.p[1] = vec3_from_3f(ti[0], h[1], tj[1]);
                    q.p[2] = vec3_from_3f(ti[1], h[1], tj[1]);
                    q.p[3] = vec3_from_3f(ti[1], h[0], tj[1]);
                    normal = quad_normal(q);

                    if(extra_normals->capacity - extra_normals->length <= 12) array_list_grow_f(extra_normals);
                    normal_unload(normal, extra_normals->data + extra_normals->length);
                    extra_normals->length += 12;

                    if(extra_vertices->capacity - extra_vertices->length <= 12) array_list_grow_f(extra_vertices);
                    quad_unload(q, extra_vertices->data + extra_vertices->length);
                    extra_vertices->length += 12;                                      
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
