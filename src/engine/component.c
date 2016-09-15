#include <stdlib.h>
#include "component.h"

Component component_init(GameObject *container,
                         ComponentUpdateCallback on_update,
                         ComponentRenderCallback on_render,
                         ComponentFreeCallback on_free) {
  Component self;

  self.container = container;
  self.allocated = 0;
  self.on_update = on_update;
  self.on_render = on_render;
  self.on_free = on_free;

  return self;
}

void component_update(Component *self, double dt) {
  if (self->on_update != NULL) {
    self->on_update(self, dt);
  }
}

void component_render(Component *self, Renderer *renderer) {
  if (self->on_render != NULL) {
    self->on_render(self, renderer);
  }
}

void component_free(Component *self) {
  if (self->on_free != NULL) {
    self->on_free(self);
  }

  if(self->allocated) {
    free(self);
  }
}
