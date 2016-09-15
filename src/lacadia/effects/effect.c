#include <stdlib.h>
#include "effect.h"

Effect effect_init(EffectType type, float duration) {
  Effect self;

  self.type = type;

  self.duration = duration;
  self.max_duration = duration;

  self.container = NULL;

  self.amount_affected = 0;

  return self;
}

void effect_free(Effect *self) {
  if (self->on_free != NULL) {
    self->on_free(self);
  }
}

void effect_apply(Effect *self, AffectableComponent *affectable) {
  self->container = affectable;
  if (self->on_apply != NULL) {
    self->on_apply(self, affectable);
  }
}

int effect_update(Effect *self, double dt) {
  self->duration -= dt;
  if (self->duration <= 0) {
    effect_end(self);
    effect_free(self);
    return 1;
  } else {
    if (self->on_update != NULL) {
      self->on_update(self, dt);
    }
    return 0;
  }
}

void effect_advance(Effect *self, double dt) {
  if (self->duration > 0 && self->on_update != NULL) {
    self->on_update(self, dt);
  }
  self->duration -= dt;
}

void effect_end(Effect *self) {
  if (self->on_end != NULL) {
    self->on_end(self);
  }
}

void effect_render(Effect *self, Renderer *renderer) {
  if (self->on_render != NULL) {
    self->on_render(self, renderer);
  }
}
