#ifndef COMPONENT_H
#define COMPONENT_H

#include "engine/game_object.h"
#include "engine/graphics/renderer.h"

typedef void (*ComponentUpdateCallback)(struct Component *self, double dt);
typedef void (*ComponentRenderCallback)(struct Component *self,
                                        Renderer *renderer);
typedef void (*ComponentFreeCallback)(void *component);

typedef struct Component {
  GameObject *container;

  int allocated;

  ComponentUpdateCallback on_update;
  ComponentRenderCallback on_render;
  ComponentFreeCallback on_free;
} Component;

Component component_init(GameObject *container,
                         ComponentUpdateCallback on_update,
                         ComponentRenderCallback on_render,
                         ComponentFreeCallback on_free);
void component_update(Component *self, double dt);
void component_render(Component *self, Renderer *renderer);
void component_free(Component *self);

#endif
