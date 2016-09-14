#include <stdlib.h>
#include "ability.h"

Ability *ability_new() {
  Ability *self = malloc(sizeof(*self));
  self->max_cooldown = 1;
  self->cooldown = 0;
  return self;
}

void ability_free(Ability *self) { free(self); }

void ability_update(Ability *self, double dt) {
  self->cooldown -= dt;
  self->cooldown = self->cooldown > 0.0 ? self->cooldown : 0.0;
}

void ability_use(Ability *self, GameWorld *world, GameObject *user) {
  self->on_use(world, user);
  self->cooldown = self->max_cooldown;
}

int ability_is_ready(Ability self) { return self.cooldown <= 0.0; }
