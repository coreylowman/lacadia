#ifndef SHADERS_H
#define SHADERS_H

#include "engine/util/vec3.h"

// forward decls
typedef struct Camera Camera;

struct Shader;

typedef void (*ShaderPreRenderCallback)(struct Shader *self, Camera camera, Vec3 clip_plane, float clip_dist);
typedef void (*ShaderRenderCallback)(struct Shader *self, Camera camera);
typedef void (*ShaderPostRenderCallback)(struct Shader *self);

typedef struct Shader {
  unsigned int vertexShader;
  unsigned int fragmentShader;
  unsigned int program;

  ShaderPreRenderCallback on_pre_render;
  ShaderRenderCallback on_render;
  ShaderPostRenderCallback on_post_render;
} Shader;

int shader_init(Shader *shader, const char *vertex_shader_loc,
                const char *fragment_shader_loc,
                ShaderPreRenderCallback on_pre_render,
                ShaderRenderCallback on_render,
                ShaderPostRenderCallback on_post_render);
void shader_render(Shader *shader, Camera camera, Vec3 clip_plane, float clip_dist);
void shader_post_render(Shader *shader);

#endif
