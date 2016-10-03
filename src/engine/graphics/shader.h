#ifndef SHADERS_H
#define SHADERS_H

#include "engine/util/mat4.h"
#include "engine/util/vec3.h"

// forward decls
struct Shader;
typedef struct Renderer Renderer;

typedef void (*ShaderPreRenderCallback)(struct Shader *self,
                                        Renderer *renderer);
typedef void (*ShaderRenderCallback)(struct Shader *self);
typedef void (*ShaderPostRenderCallback)(struct Shader *self);

typedef struct Shader {
  unsigned int vertexShader;
  unsigned int fragmentShader;
  unsigned int program;

  ShaderPreRenderCallback on_pre_render;
  ShaderRenderCallback on_render;
  ShaderPostRenderCallback on_post_render;

  int projection_matrix_location;
  int view_matrix_location;
  int light_position_location;
  int clip_plane_location;
} Shader;

int shader_init(Shader *shader, const char *vertex_shader_loc,
                const char *fragment_shader_loc,
                ShaderPreRenderCallback on_pre_render,
                ShaderRenderCallback on_render,
                ShaderPostRenderCallback on_post_render);
void shader_render(Shader *shader, Renderer *renderer);
void shader_post_render(Shader *shader);

#endif
