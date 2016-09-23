#ifndef MODEL_SHADER_H
#define MODEL_SHADER_H

#include "engine/graphics/shader.h"
#include "engine/util/object_model.h"
#include "engine/util/array_list.h"
#include "engine/util/mat4.h"
#include "engine/asset_manager.h"

#define MAX_MODELS 32

typedef struct ModelShader {
  Shader base_shader;

  unsigned int vbo[2], vao;
  int num_models;
  ObjectModel *models[MAX_MODELS];
  ArrayList_m4 *model_model_matrices[MAX_MODELS]; // ArrayList<Mat4>[MAX_MODELS]

  int projection_matrix_location;
  int view_matrix_location;
  int light_position_location;
  int clip_plane_location;
} ModelShader;

ModelShader *model_shader_new(AssetManager *asset_manager);
void model_shader_free(ModelShader *self);
void model_shader_add_model(ModelShader *self, int model_id, Mat4 model_matrix);

#endif
