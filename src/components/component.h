#ifndef COMPONENT_H
#define COMPONENT_H

#include "game/game_object.h"
#include "util/renderer.h"

typedef void (*ComponentUpdateCallback)(struct Component *self, double dt);
typedef void (*ComponentRenderCallback)(struct Component *self,
                                        Renderer *renderer);
typedef void (*ComponentFreeCallback)(void *component);

typedef enum ComponentType {
  COMPONENT_TYPE_COLLIDABLE,
  COMPONENT_TYPE_RENDERABLE,
  COMPONENT_TYPE_AFFECTABLE,
  COMPONENT_TYPE_MAX,
} ComponentType;

typedef struct Component {
  GameObject *container;

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
