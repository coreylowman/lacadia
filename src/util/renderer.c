#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include "lodepng.h"
#include "renderer.h"
#include "util/string_helpers.h"

Vec3 light_position = { .data = { 0, 100, 0 } };

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

Renderer *renderer_new() {
    Renderer *self = malloc(sizeof(*self));

    //
    //MODELS
    //
    shader_init(&self->model_shader, "shaders/model_vert.glsl", "shaders/model_frag.glsl");
    self->num_models = 9;
    self->model_names[0] = "assets/default_box";
    self->model_names[1] = "assets/mage";
    self->model_names[2] = "assets/hunter";
    self->model_names[3] = "assets/bug";
    self->model_names[4] = "assets/fireball";
    self->model_names[5] = "assets/icicle";
    self->model_names[6] = "assets/burn_particle";
    self->model_names[7] = "assets/wall";
    self->model_names[8] = "assets/sphere";

    printf("Loading assets... ");
    int i;
    for(i = 0;i < self->num_models;i++){
        self->models[i] = obj_model_from_file(self->model_names[i]);        
        self->model_model_matrices[i] = array_list_new_m4();
    }
    printf("done.\n");

    glGenVertexArrays(1, &self->model_vao);
    glBindVertexArray(self->model_vao);
    
    glGenBuffers(3, &self->model_vbo[0]);
    
    //vertex data buffer object
    glBindBuffer(GL_ARRAY_BUFFER, self->model_vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    //color data buffer object
    glBindBuffer(GL_ARRAY_BUFFER, self->model_vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, self->model_vbo[2]);
    //instance model matrix buffer object
    for(i = 0;i < 4;i++){
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void *)(4 * i * sizeof(float)));
        glEnableVertexAttribArray(2 + i);
        glVertexAttribDivisor(2 + i, 1);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //
    // UI
    //
    shader_init(&self->ui_shader, "shaders/ui_vert.glsl", "shaders/ui_frag.glsl");
    self->num_ui_rects = 0;
    //ui vbo/vao
    glGenVertexArrays(1, &self->ui_vao);
    glBindVertexArray(self->ui_vao);
    
    glGenBuffers(2, &self->ui_vbo[0]);
    
    //vertex data buffer object
    glBindBuffer(GL_ARRAY_BUFFER, self->ui_vbo[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    //color data buffer object
    glBindBuffer(GL_ARRAY_BUFFER, self->ui_vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //
    // LINES
    //
    shader_init(&self->line_shader, "shaders/line_vert.glsl", "shaders/line_frag.glsl");
    self->num_lines = 0;
    glGenVertexArrays(1, &self->line_vao);
    glBindVertexArray(self->line_vao);
    
    glGenBuffers(1, &self->line_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, self->line_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //
    // TERRAINS
    //
    shader_init(&self->terrain_shader, "shaders/terrain_vert.glsl", "shaders/terrain_frag.glsl");
	self->num_terrains = 0;
    glGenVertexArrays(1, &self->terrain_vao);
    glBindVertexArray(self->terrain_vao);
    
    glGenBuffers(1, &self->terrain_vbo);
    
    //vertex data buffer object
    glBindBuffer(GL_ARRAY_BUFFER, self->terrain_vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (const GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (const GLvoid *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (const GLvoid *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned png_width, png_height;

    //size should be 256x256
    if(!load_png_data("assets/grass.2d.png", &self->textures[0], &png_width, &png_height)){
        printf("error: cannot load \"assets/grass.2d.png\"\n");
    }

    if(!load_png_data("assets/grass_dirt.2d.png", &self->textures[1], &png_width, &png_height)){
        printf("error: cannot load \"assets/grass_dirt.2d.png\"\n");
    }

    if(!load_png_data("assets/dirt.2d.png", &self->textures[2], &png_width, &png_height)){
        printf("error: cannot load \"assets/dirt.2d.png\"\n");
    }

	glGenTextures(3, &self->texture_ids[0]);

	glBindTexture(GL_TEXTURE_2D, self->texture_ids[0]);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, self->textures[0]);

    glBindTexture(GL_TEXTURE_2D, self->texture_ids[1]); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, self->textures[1]);

    glBindTexture(GL_TEXTURE_2D, self->texture_ids[2]); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, self->textures[2]);

	glBindTexture(GL_TEXTURE_2D, 0);

    return self;
}

void renderer_free(Renderer *self){
    int i;
    for (i = 0; i < self->num_models; i++){
        obj_model_free(self->models[i]);
        array_list_free_m4(self->model_model_matrices[i]);
    }
    free(self->textures[0]);
    free(self->textures[1]);
    free(self->textures[2]);
	free(self);
}

int renderer_get_model_id(Renderer *self, const char *name){
    int i;
    for(i = 0;i < self->num_models;i++){
        if(string_equals(self->model_names[i], name))
            return i;
    }
    //default to default box
    return 0;
}

Obb renderer_get_model_obb(Renderer *self, int model_id){
    return self->models[model_id]->bounding_box;
}

void renderer_render(Renderer *self, Mat4 projection_matrix, Mat4 view_matrix){
    //render models
    glUseProgram(self->model_shader.program);
    glUniformMatrix4fv(self->model_shader.projection_matrix_location, 1, GL_TRUE, &projection_matrix.data[0]);
    glUniformMatrix4fv(self->model_shader.view_matrix_location, 1, GL_TRUE, &view_matrix.data[0]);
    
    int i;
    for(i = 0;i < self->num_models;i++){
        ObjectModel *model = self->models[i];
        ArrayList_m4 *model_matrices = self->model_model_matrices[i];
        
        if (model_matrices->length == 0) continue;

        //upload vertices of model
        glBindBuffer(GL_ARRAY_BUFFER, self->model_vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, model->num_floats * sizeof(float), &model->vertices[0], GL_STATIC_DRAW);

        //upload colors of model
        glBindBuffer(GL_ARRAY_BUFFER, self->model_vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, model->num_floats * sizeof(float), &model->colors[0], GL_STATIC_DRAW);

        //upload instace model matrices
        glBindBuffer(GL_ARRAY_BUFFER, self->model_vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, model_matrices->length * sizeof(Mat4), &model_matrices->data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        //draw stuff
        glBindVertexArray(self->model_vao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, model->num_floats / 3, model_matrices->length);
        glBindVertexArray(0);

        self->model_model_matrices[i]->length = 0;
    }

    //render ui rects
    glUseProgram(self->ui_shader.program);

    //upload vertices of model
    glBindBuffer(GL_ARRAY_BUFFER, self->ui_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 8 * self->num_ui_rects * sizeof(float), &self->rects[0], GL_STATIC_DRAW);

    //upload colors of model
    glBindBuffer(GL_ARRAY_BUFFER, self->ui_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 12 * self->num_ui_rects * sizeof(float), &self->rect_colors[0], GL_STATIC_DRAW);

    glBindVertexArray(self->ui_vao);
    glDrawArrays(GL_QUADS, 0, 4 * self->num_ui_rects);
    glBindVertexArray(0);

    self->num_ui_rects = 0;

    //render lines
    glUseProgram(self->line_shader.program);
    glUniformMatrix4fv(self->line_shader.projection_matrix_location, 1, GL_TRUE, &projection_matrix.data[0]);
    glUniformMatrix4fv(self->line_shader.view_matrix_location, 1, GL_TRUE, &view_matrix.data[0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, self->line_vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * 3 * self->num_lines * sizeof(float), &self->lines[0], GL_STATIC_DRAW);

    glBindVertexArray(self->line_vao);
    glDrawArrays(GL_LINES, 0, 2 * self->num_lines);
    glBindVertexArray(0);
    
	self->num_lines = 0;

    //render terrain
    glUseProgram(self->terrain_shader.program);
    glUniformMatrix4fv(self->terrain_shader.projection_matrix_location, 1, GL_TRUE, &projection_matrix.data[0]);
    glUniformMatrix4fv(self->terrain_shader.view_matrix_location, 1, GL_TRUE, &view_matrix.data[0]);
	glUniform3f(self->terrain_shader.light_position_location, light_position.x, light_position.y, light_position.z);
    
    glUniform1i(self->terrain_shader.texture_location, 0);
    glActiveTexture(GL_TEXTURE0);

    Terrain t;
    for(i = 0;i < self->num_terrains;i++){
        t = self->terrains[i];
        glBindVertexArray(self->terrain_vao);
        glBindBuffer(GL_ARRAY_BUFFER, self->terrain_vbo);

        glBufferData(GL_ARRAY_BUFFER, t.num_grass_vertices * sizeof(TerrainVertex), &t.grass_vertices[0], GL_STATIC_DRAW);
        glBindTexture(GL_TEXTURE_2D, self->texture_ids[0]);
        glDrawArrays(GL_QUADS, 0, t.num_grass_vertices);
        
        if(t.num_grass_dirt_vertices > 0){
            glBufferData(GL_ARRAY_BUFFER, t.num_grass_dirt_vertices * sizeof(TerrainVertex), &t.grass_dirt_vertices[0], GL_STATIC_DRAW);
            glBindTexture(GL_TEXTURE_2D, self->texture_ids[1]);
            glDrawArrays(GL_QUADS, 0, t.num_grass_dirt_vertices);            
        }
        
        if(t.num_dirt_vertices > 0){
            glBufferData(GL_ARRAY_BUFFER, t.num_dirt_vertices * sizeof(TerrainVertex), &t.dirt_vertices[0], GL_STATIC_DRAW);
            glBindTexture(GL_TEXTURE_2D, self->texture_ids[2]);
            glDrawArrays(GL_QUADS, 0, t.num_dirt_vertices);            
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

	self->num_terrains = 0;
}

void renderer_render_model(Renderer *self, int model_id, Mat4 model_matrix){
    array_list_push_m4(self->model_model_matrices[model_id], model_matrix);
}

void renderer_render_rect(Renderer *self, Rect2 rect, Vec3 color){
    int i = self->num_ui_rects;
    if(i < MAX_UI_RECTS){
        self->rects[i] = ui_rect_from_rect2(rect);
        self->rect_colors[4*i] = color;
        self->rect_colors[4*i+1] = color;
        self->rect_colors[4*i+2] = color;
        self->rect_colors[4*i+3] = color;
        self->num_ui_rects++;
    }else{
        printf("error! too many rects\n");
    }
}

void renderer_render_line(Renderer *self, Line line){
    int i = self->num_lines;
    if(i < MAX_LINES){
        self->lines[i] = line;
        self->num_lines++;
    }else {
        printf("error! too many lines\n");
    }
}

void renderer_render_terrain(Renderer *self, Terrain terrain){
    int i = self->num_terrains;
    if(i < MAX_TERRAINS){
        self->terrains[i] = terrain;
        self->num_terrains++;
    }else {
        printf("error! too many lines\n");
    }
}

void renderer_render_sphere(Renderer *self, Vec3 position){
    int model_id = renderer_get_model_id(self, "assets/sphere");
    Mat4 model_matrix;
    mat4_ident(&model_matrix);
    mat4_translate(&model_matrix, position);
    mat4_transpose(&model_matrix);
    renderer_render_model(self, model_id, model_matrix);
}

