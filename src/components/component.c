#include "component.h"

Component component_init(GameObject *container, ComponentUpdateCallback on_update, ComponentRenderCallback on_render) {
    Component self;

    self.container = container;
    self.on_update = on_update;
    self.on_render = on_render;

    return self;
}

void component_update(Component *self, double dt){
    self->on_update(self, dt);
}

void component_render(Component *self, Renderer *renderer){
    self->on_render(self, renderer);
}
