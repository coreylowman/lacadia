#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "terrain.h"
#include "simplex_noise.h"

#define TERRAIN_SIZE 128
#define OCTAVES 4
#define JAGGEDNESS 5.0
#define DAMPENING 2.0

static float height_map[TERRAIN_SIZE][TERRAIN_SIZE];

Terrain terrain_new(Vec3 start, Vec3 dimensions){
    Terrain self;

    glGenVertexArrays(1, &self.asset_vao);
    glBindVertexArray(self.asset_vao);
    
    glGenBuffers(2, &self.asset_vbo[0]);
    
    //vertex data buffer object
    glBindBuffer(GL_ARRAY_BUFFER, self.asset_vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, self.asset_vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    int i, j, k;
    float tx, tz;
    float octave_vals[OCTAVES];
    float octave_ivals[OCTAVES];
    float inv_damp = 1.0 / DAMPENING;

    for(i = 0;i < OCTAVES;i++){
        octave_vals[i] = pow(inv_damp, i);
        octave_ivals[i] = 1.0 / octave_vals[i];
    }

    for(i = 0;i < TERRAIN_SIZE;i++){
        for(j = 0;j < TERRAIN_SIZE;j++){
            tx = JAGGEDNESS * i / dimensions.x;
            tz = JAGGEDNESS * j / dimensions.z;
            
            height_map[i][j] = 0;

            for(k = 0;k < OCTAVES;k++){
                height_map[i][j] += octave_ivals[k] * simplex_noise(start.x + octave_vals[k] * tx,start.z + octave_vals[k] * tz);
            }            
        }
    }

    //int width = dimensions.x, length = dimensions.z;
    //for each row
    //for each point
    //generate a tri going from (x,y) to (x+1,y) to (x,y+1)
    //e.g. ---
    //     | /
    //then generate a tri going from (x,y+1) to (x+1,y) to (x+1,y+1)
    //e.g. / |
    //     ---
    //this will generate a square for each point
    self.num_vertices = 2 * 3 * 3 * (TERRAIN_SIZE - 1) * (TERRAIN_SIZE - 1); 
    self.vertices = malloc(self.num_vertices * sizeof(float));
    self.normals = malloc(self.num_vertices * sizeof(float));
    int ind;
    Vec3 a, b, e1, e2, normal;
    for(i = 0;i < TERRAIN_SIZE - 1;i++){
        for(j = 0;j < TERRAIN_SIZE - 1;j++){
            ind = 18 * (i + j * (TERRAIN_SIZE - 1));
            
            //triangle 1
            a = vec3_from_3f(i, height_map[i][j], j);
            b = vec3_from_3f(i+1, height_map[i+1][j], j);
            e1 = vec3_sub(b , a);
            b = vec3_from_3f(i, height_map[i][j+1], j+1);
            e2 = vec3_sub(b , a);
            normal = vec3_cross(e1,e2);
            vec3_normalize(&normal);
            for(k = 0;k < 3;k++){
                self.normals[ind + k * 3 + 0] = normal.x;
                self.normals[ind + k * 3 + 1] = normal.y;
                self.normals[ind + k * 3 + 2] = normal.z;
            }

            self.vertices[ind + 0] = i;
            self.vertices[ind + 1] = height_map[i][j];
            self.vertices[ind + 2] = j;
            
            self.vertices[ind + 3] = i+1;
            self.vertices[ind + 4] = height_map[i+1][j];
            self.vertices[ind + 5] = j;
            
            self.vertices[ind + 6] = i;
            self.vertices[ind + 7] = height_map[i][j+1];
            self.vertices[ind + 8] = j+1;
            

            //triangle 2
            a = vec3_from_3f(i, height_map[i][j+1], j+1);
            b = vec3_from_3f(i+1, height_map[i+1][j], j);
            e1 = vec3_sub(b , a);
            b = vec3_from_3f(i+1, height_map[i+1][j+1], j+1);
            e2 = vec3_sub(b , a);
            normal = vec3_cross(e1,e2);
            vec3_normalize(&normal);
            for(k = 0;k < 3;k++){
                self.normals[ind + 9 + k * 3 + 0] = normal.x;
                self.normals[ind + 9 + k * 3 + 1] = normal.y;
                self.normals[ind + 9 + k * 3 + 2] = normal.z;
            }

            self.vertices[ind + 9] = i;
            self.vertices[ind + 10] = height_map[i][j+1];
            self.vertices[ind + 11] = j+1;
            
            self.vertices[ind + 12] = i+1;
            self.vertices[ind + 13] = height_map[i+1][j];
            self.vertices[ind + 14] = j;
            
            self.vertices[ind + 15] = i+1;
            self.vertices[ind + 16] = height_map[i+1][j+1];
            self.vertices[ind + 17] = j+1;
        }
    }

    return self;
}

void terrain_free(Terrain self){
    free(self.vertices);
	free(self.normals);
}

void terrain_render(Terrain self){
    glBindBuffer(GL_ARRAY_BUFFER, self.asset_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, self.num_vertices * sizeof(float), &self.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, self.asset_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, self.num_vertices * sizeof(float), &self.normals[0], GL_STATIC_DRAW);

    glBindVertexArray(self.asset_vao);
    glDrawArrays(GL_TRIANGLES, 0, self.num_vertices / 3);
    glBindVertexArray(0);
}
