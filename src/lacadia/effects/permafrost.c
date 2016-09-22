#include <stdlib.h>
#include "permafrost.h"
#include "engine/util/random.h"
#include "engine/graphics/renderer.h"
#include "lacadia/enemies/enemy.h"
#include "lacadia/effects/affectable_component.h"

static void permafrost_on_apply(Effect *self, AffectableComponent *affectable);
static void permafrost_on_update(Effect *self, double dt);
static void permafrost_on_render(Effect *self, Renderer *renderer);
static void permafrost_on_end(Effect *self);
static void permafrost_on_free(Effect *self);

static void permafrost_particle_init(Particle *p, Vec3 position,
                                     float duration) {
  // want it to form an outwardly flowing circle
  float t = random_in_rangef(0, 1);
  Vec3 direction = random_flat_vec3(1.0);
  Vec3 displacement = vec3_scale(direction, 1 + t);
  p->position = vec3_add(position, displacement);
  p->velocity = vec3_scale(direction, 2.5);
  p->duration = (1 - t) * duration;
}

Permafrost *permafrost_new(GameWorld *world, GameObject *target, float dmg,
                           float slow_pct, float duration) {
  Permafrost *self = malloc(sizeof(*self));

  self->base_effect = effect_init(EFFECT_TYPE_PERMAFROST, duration);

  self->damage = dmg;
  self->slow_pct = slow_pct;

  self->particle_system =
      particle_system_new(world, target->position, "./assets/frost_particle", 64,
                          duration, duration * 0.2);
  particle_system_set_particle_init(self->particle_system,
                                    permafrost_particle_init);
  particle_system_set_follow_target(self->particle_system, target);

  self->base_effect.on_apply = permafrost_on_apply;
  self->base_effect.on_update = permafrost_on_update;
  self->base_effect.on_render = permafrost_on_render;
  self->base_effect.on_end = permafrost_on_end;
  self->base_effect.on_free = permafrost_on_free;

  return self;
}

static void permafrost_on_apply(Effect *self, AffectableComponent *affectable) {
  Permafrost *permafrost =
      (Permafrost *)affectable->effects[EFFECT_TYPE_PERMAFROST];
  if (permafrost != NULL) {
    // if permafrost is already on the target, damage and reset cd on slow
    permafrost->base_effect.duration = permafrost->base_effect.max_duration;

    // free the effect we were trying to add, since we just modified the
    // permafrost
    // already on the target
    effect_free(self);
  } else {
    permafrost = (Permafrost *)self;

    // there is no permafrost already on the target... add it
    affectable->effects[EFFECT_TYPE_PERMAFROST] = self;

    float val = affectable->speed * permafrost->slow_pct;
    self->amount_affected = affectable_component_slow(affectable, val);
  }

  float val = affectable_component_damage(affectable, permafrost->damage);
}

static void permafrost_on_update(Effect *effect, double dt) {
  Permafrost *self = (Permafrost *)effect;

  particle_system_update((GameObject *)self->particle_system, dt);
}

static void permafrost_on_render(Effect *effect, Renderer *renderer) {
  Permafrost *self = (Permafrost *)effect;
  particle_system_render((GameObject *)self->particle_system, renderer);
}

static void permafrost_on_end(Effect *self) {
  // undo the slow
  self->amount_affected -=
      affectable_component_quicken(self->container, self->amount_affected);
}

static void permafrost_on_free(Effect *effect) {
  Permafrost *self = (Permafrost *)effect;
  particle_system_free((GameObject *)self->particle_system);
  free(self);
}
