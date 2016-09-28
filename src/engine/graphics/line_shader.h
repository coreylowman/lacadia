#ifndef LINE_SHADER_H
#define LINE_SHADER_H

#include "engine/graphics/shader.h"
#include "engine/util/line.h"

#define MAX_LINES 2048

typedef struct LineShader {
  Shader base_shader;

  int num_lines;
  Line lines[MAX_LINES];
  unsigned int vbo, vao;
} LineShader;

LineShader *line_shader_new();
void line_shader_free(LineShader *self);

void line_shader_add_line(LineShader *self, Line line);

#endif
