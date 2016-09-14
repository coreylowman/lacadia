#include <stdio.h>
#include <stdlib.h>
#include "spell.h"
#include "util/obb.h"

Spell *spell_new(GameWorld *world, GameObjectUpdateCallback on_update,
                 GameObjectRenderCallback on_render,
                 GameObjectFreeCallback on_free) {
  Spell *self = malloc(sizeof(*self));
  self->base_object = game_object_init(world, GAME_OBJECT_TYPE_SPELL, on_update,
                                       on_render, on_free);
  return self;
}

void spell_free(GameObject *obj) {
  Spell *self = (Spell *)obj;
  component_free((Component *)&self->collidable);
  component_free((Component *)&self->renderable);
  free(self);
}

void spell_update(GameObject *obj, double dt) {
  Spell *self = (Spell *)obj;
  component_update((Component *)&self->collidable, dt);
  component_update((Component *)&self->renderable, dt);
  game_object_move((GameObject *)&self->base_object, self->speed * dt);
}

void spell_render(GameObject *obj, Renderer *renderer) {
  Spell *self = (Spell *)obj;
  component_render((Component *)&self->renderable, renderer);
  component_render((Component *)&self->collidable, renderer);
}

int spell_is_colliding(CollidableComponent self, CollidableComponent other) {
  Spell *spell = (Spell *)self.base_component.container;
  if (spell->caster_type != other.base_component.container->type) {
    return obb_intersects(self.bounding_box, other.bounding_box);
  }
  return 0;
}

int spell_is_colliding_with_target(CollidableComponent self,
                                   CollidableComponent other) {
  Spell *spell = (Spell *)self.base_component.container;
  if (spell->target == other.base_component.container) {
    return obb_intersects(self.bounding_box, other.bounding_box);
  }
  return 0;
}
