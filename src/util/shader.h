#ifndef SHADERS_H
#define SHADERS_H

typedef struct Shader {
  unsigned int vertexShader;
  unsigned int fragmentShader;
  unsigned int program;

  int projection_matrix_location;
  int view_matrix_location;
  int light_position_location;
  int texture_location;
} Shader;

int shader_init(Shader *shader, const char *vertex_shader_loc,
                const char *fragment_shader_loc);

#endif
