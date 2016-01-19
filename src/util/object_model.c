#include <string.h>
#include <stdio.h>
#include <float.h>
#include "lodepng.h"
#include "object_model.h"
#include "array_list.h"
#include "mat4.h"

#define OBJECT_MODEL_SCALE 0.05

typedef struct {
    unsigned int vertex_indices[3];
    unsigned int texture_indices[3];
    unsigned int normal_indices[3];
} Face;

static Face *face_new(float *p, float *t, float *n){
    Face *self = malloc(sizeof(*self));
    int i;
    for(i = 0;i < 3;i++){
        self->vertex_indices[i] = p[i];
        self->texture_indices[i] = t[i];
        self->normal_indices[i] = n[i];
    }
    return self;
}

static void face_free(void *data){
    free(data);
}

typedef struct {
    float x,y,z;
} Vertex;

static Vertex *vertex_new(float *p){
    Vertex *self = malloc(sizeof(*self));
    self->x = p[0];
    self->y = p[1];
    self->z = p[2];
    return self;
}

static void vertex_free(void *data){
    free(data);
}

static int load_png_data(const char *filename, unsigned char **image, unsigned *width, unsigned * height){
    unsigned error;
    unsigned char* png = 0;
    size_t pngsize;

    error = lodepng_load_file(&png, &pngsize, filename);
    if(!error) error = lodepng_decode32(image, width, height, png, pngsize);
    if(error){
        printf("error %u: %s\n", error, lodepng_error_text(error));  
        free(png);
        return 0;
    }

    free(png);
    return 1;
}

ObjectModel *obj_model_from_file(const char *filename){
    ArrayList *face_list = array_list_new(face_free);
    ArrayList *vertex_list = array_list_new(vertex_free);
    ArrayList *normal_list = array_list_new(vertex_free);
    ArrayList *texture_list = array_list_new(vertex_free);

    char line[128];
    char *token;
    float p[3];
    float t[3]; //used for parsing faces
    float n[3]; //used for parsing faces
    int i;

    char obj_filename[128];
    sprintf(obj_filename, "%s.obj", filename);

    FILE *input = fopen(obj_filename, "r");
	if (input == NULL){
		printf("error: file %s not opened\n", filename);
    }

    while(fgets(line, 128, input) != NULL){
        token = strtok(line, " ");
        if(strcmp(token, "vn") == 0){
            for(i = 0;i < 3;i++){
                token = strtok(NULL, " ");
                n[i] = atof(token);
            }
            array_list_push(normal_list, vertex_new(n));
		}
		else if (strcmp(token, "vt") == 0){
			for (i = 0; i < 2; i++){
				token = strtok(NULL, " ");
				t[i] = atof(token);
			}
			t[i] = 0;
			array_list_push(texture_list, vertex_new(t));
		}else if (strcmp(token, "v") == 0){
			for (i = 0; i < 3; i++){
				token = strtok(NULL, " ");
				p[i] = atof(token);
			}
			array_list_push(vertex_list, vertex_new(p));
        }else if(strcmp(token, "f") == 0){
            for(i = 0;i < 3;i++){
                token = strtok(NULL, "/");
                p[i] = atof(token);
                token = strtok(NULL, "/");
                t[i] = atof(token);
                token = strtok(NULL, " ");
                n[i] = atof(token);
            }
            array_list_push(face_list, face_new(p,t,n));            
        }else if(strcmp(token, "mtllib") == 0){
 
        }else if(strcmp(token, "usemtl") == 0){
 
        }
    }
    fclose(input);

    unsigned char *png_data;
    unsigned png_width, png_height;
    char png_filename[128];
    sprintf(png_filename, "%s.png", filename);

    if(!load_png_data(png_filename, &png_data, &png_width, &png_height)){
        printf("error: lodepng error");
    }

    ObjectModel *self = malloc(sizeof(*self));
    self->num_floats = 9 * face_list->length;
    self->vertices = malloc(self->num_floats * sizeof(float));
    self->colors = malloc(self->num_floats * sizeof(float));

    Face *f;
    Vertex *v;
    int j, k;
	int a, b, c;
	int tx, ty;
    float inv_255 = 1.0 / 255.0;

    float min[3], max[3];
    float q[3];
    for(i = 0;i < 3;i++){
        min[i] = FLT_MAX;
        max[i] = FLT_MIN;
    }

    for(i = 0;i < face_list->length;i++){
        f = face_list->data[i];
        for(j = 0;j < 3;j++){
			a = 9 * i + j * 3 + 0;
			b = a + 1;
			c = a + 2;
            v = vertex_list->data[f->vertex_indices[j] - 1];

            q[0] = v->x * OBJECT_MODEL_SCALE;
            q[1] = v->y * OBJECT_MODEL_SCALE;
            q[2] = v->z * OBJECT_MODEL_SCALE;

            self->vertices[a] = q[0];
            self->vertices[b] = q[1];
            self->vertices[c] = q[2];

            for(k = 0;k < 3;k++){
                if(q[k] < min[k]) min[k] = q[k];
                if(q[k] > max[k]) max[k] = q[k];
            }

			v = texture_list->data[f->texture_indices[j] - 1];
			tx = v->x * png_width;
            ty = v->y * png_height;
            self->colors[a] = png_data[4 * png_width * ty + 4 * tx + 0] * inv_255; //r
            self->colors[b] = png_data[4 * png_width * ty + 4 * tx + 1] * inv_255; //b
            self->colors[c] = png_data[4 * png_width * ty + 4 * tx + 2] * inv_255; //g
        }
    }

	free(png_data);
    array_list_free(face_list);
    array_list_free(vertex_list);
    array_list_free(texture_list);
    array_list_free(normal_list);

    self->bounding_box.x = (max[0] + min[0]) * 0.5;
    self->bounding_box.y = (max[1] + min[1]) * 0.5;
    self->bounding_box.z = (max[2] + min[2]) * 0.5;

    self->bounding_box.width = (max[0] - min[0]) * 0.5;
    self->bounding_box.height = (max[1] - min[1]) * 0.5;
    self->bounding_box.length = (max[2] - min[2]) * 0.5;

    return self;
}

//to use with global object models
ObjectModel *obj_model_clone(ObjectModel *self){
    if(self == NULL) return NULL;

    ObjectModel *other = malloc(sizeof(*other));

    other->num_floats = self->num_floats;

    other->vertices = malloc(self->num_floats * sizeof(float));
    memcpy(other->vertices, self->vertices, self->num_floats);

    other->colors = malloc(self->num_floats * sizeof(float));
    memcpy(other->colors, self->colors, self->num_floats);

    return other;
}

void obj_model_free(ObjectModel *self){
	if (self == NULL) return;
    free(self->vertices);
    free(self->colors);
    free(self);
}

