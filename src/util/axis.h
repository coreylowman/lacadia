#ifndef AXIS_H
#define AXIS_H

#include "shader.h"

typedef struct {
    unsigned int vbo[3], vao;
} Axis;

void axis_init(Axis *self);
void axis_render(Axis self, Shader shader);

#endif
