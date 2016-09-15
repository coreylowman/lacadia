#include <stdio.h>
#include <stdlib.h>
#include "spell.h"
#include "engine/util/obb.h"

Spell *spell_new(GameWorld *world, GameObjectUpdateCallback on_update,
                 GameObjectRenderCallback on_render,
                 GameObjectFreeCallback on_free) {
  Spell *self = malloc(sizeof(*self));
  self->base_object =
      game_object_init(world, "spell", on_update, on_render, on_free);
  return self;
}

void spell_free(GameObject *obj) {
  Spell *self = (Spell *)obj;
  free(self);
}

void spell_update(GameObject *obj, double dt) {
  Spell *self = (Spell *)obj;
  game_object_move((GameObject *)&self->base_object, self->speed * dt);
}

void spell_render(GameObject *obj, Renderer *renderer) {
  Spell *self = (Spell *)obj;
}

int spell_is_colliding(CollidableComponent self, CollidableComponent other) {
  Spell *spell = (Spell *)self.base_component.container;
  if (strcmp(spell->caster_tag, other.base_component.container->tag) != 0) {
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
