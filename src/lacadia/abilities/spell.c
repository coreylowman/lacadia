#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "spell.h"
#include "engine/util/obb.h"

Spell *spell_new(GameWorld *world, GameObject *user, const char *asset_name,
                 OnCollideCallback on_collide) {
  Spell *self = malloc(sizeof(*self));
  self->base_object =
      game_object_init(world, "spell", spell_update, spell_render, spell_free);

  game_object_alloc_components(&self->base_object, 2);
  self->base_object.components[0] = (Component *)renderable_component_new(
      &self->base_object, asset_name, world->renderer);
  self->base_object.components[1] = (Component *)collidable_component_new(
      &self->base_object,
      game_world_get_model_obb(world, self->renderable->model_id), on_collide);
  self->collidable->is_colliding = spell_is_colliding;
  self->renderable = (RenderableComponent *)self->base_object.components[0];
  self->collidable = (CollidableComponent *)self->base_object.components[1];

  strcpy(self->caster_tag, user->tag);

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
