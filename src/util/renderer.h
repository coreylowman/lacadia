#ifndef RENDERER_H
#define RENDERER_H

#include "util/shader.h"
#include "util/object_model.h"
#include "util/array_list.h"
#include "util/rect.h"
#include "util/mat4.h"
#include "util/line.h"
#include "util/obb.h"
#include "util/terrain.h"

#define MAX_ASSETS 32
#define MAX_UI_RECTS 128
#define MAX_LINES 2048
#define MAX_TERRAINS 4

typedef struct Renderer {

    //models and stuff aka assets
    Shader model_shader;
    int num_models;
    char *model_names[MAX_ASSETS];
    ObjectModel *models[MAX_ASSETS];
    ArrayList_m4 *model_model_matrices[MAX_ASSETS]; //ArrayList<Mat4>[MAX_ASSETS]
    unsigned int model_vbo[3], model_vao;

    //ui rects and stuff
    Shader ui_shader;
    int num_ui_rects;
    UIRect rects[MAX_UI_RECTS];
    Vec3 rect_colors[4 * MAX_UI_RECTS];
    unsigned int ui_vbo[2], ui_vao;

    //lines and stuff
    Shader line_shader;
    int num_lines;
    Line lines[MAX_LINES];
    unsigned int line_vbo, line_vao;

    //terrains and stuff
    Shader terrain_shader;
    int num_terrains;
    Terrain terrains[MAX_TERRAINS];
    unsigned int terrain_vbo[4], terrain_vao;
    unsigned char *textures[3];
    unsigned int texture_ids[3];
} Renderer;

Renderer *renderer_new();
void renderer_free(Renderer *self);

int renderer_get_model_id(Renderer *self, const char *name);
Obb renderer_get_model_obb(Renderer *self, int model_id);

void renderer_render(Renderer *self, Mat4 projection_matrix, Mat4 view_matrix);

void renderer_render_model(Renderer *self, int model_id, Mat4 model_matrix);
void renderer_render_rect(Renderer *self, Rect2 rect, Vec3 color);
void renderer_render_line(Renderer *self, Line line);
void renderer_render_terrain(Renderer *self, Terrain terrain);
void renderer_render_sphere(Renderer *self, Vec3 position);

#endif
