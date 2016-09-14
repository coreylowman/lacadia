#ifndef RENDERABLE_COMPONENT_H
#define RENDERABLE_COMPONENT_H

#include "component.h"
#include "util/renderer.h"
#include "util/mat4.h"

typedef struct RenderableComponent {
  Component base_component;

  float scale;

  int model_id;
  Mat4 model_matrix;
} RenderableComponent;

RenderableComponent renderable_component_init(GameObject *container,
                                              const char *model_name,
                                              Renderer *renderer);
void renderable_component_set_model_matrix(RenderableComponent *self,
                                           Mat4 model_matrix);
void renderable_component_set_scale(RenderableComponent *self, float scale);

#endif
