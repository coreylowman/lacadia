#include <stdlib.h>
#include "players/player.h"
#include "soulburn.h"
#include "engine/util/random.h"
#include "engine/util/renderer.h"
#include "engine/affectable_component.h"

static void soulburn_on_apply(Effect *self, AffectableComponent *affectable);
static void soulburn_on_update(Effect *self, double dt);
static void soulburn_on_render(Effect *self, Renderer *renderer);
static void soulburn_on_free(Effect *self);

// TODO make this look different - maybe have it go back towards the user
// todo be able to add custom args to particle_init
static void soulburn_particle_init(Particle *p, Vec3 position, float duration) {
  p->position = vec3_add(position, random_length_vec3(1.0));
  p->velocity = (Vec3){.data = {0, 1, 0}};
  p->duration = random_in_rangef(0, duration);
}

Soulburn *soulburn_new(GameWorld *world, GameObject *target, GameObject *user,
                       float dps, float leech_pct, float duration) {
  Soulburn *self = malloc(sizeof(*self));

  self->base_effect = effect_init(EFFECT_TYPE_SOULBURN, duration);

  self->leech_pct = leech_pct;
  self->dps = dps;
  self->user = user;

  self->particle_system =
      particle_system_new(world, target->position, "assets/burn_particle", 16,
                          duration, duration * 0.4);
  particle_system_set_particle_init(self->particle_system,
                                    soulburn_particle_init);
  particle_system_set_follow_target(self->particle_system, target);

  self->base_effect.on_apply = soulburn_on_apply;
  self->base_effect.on_update = soulburn_on_update;
  self->base_effect.on_render = soulburn_on_render;
  self->base_effect.on_end = NULL;
  self->base_effect.on_free = soulburn_on_free;

  return self;
}

static void soulburn_on_apply(Effect *self, AffectableComponent *affectable) {
  Soulburn *soulburn = (Soulburn *)affectable->effects[EFFECT_TYPE_SOULBURN];

  if (soulburn != NULL) {
    // refresh the duration of the existing soulburn
    soulburn->particle_system->duration = soulburn->base_effect.max_duration;
    soulburn->base_effect.duration = soulburn->base_effect.max_duration;

    // free the effect we were trying to add, since we just modified the
    // soulburn
    // already on the target
    effect_free(self);
    return;
  } else {
    affectable->effects[EFFECT_TYPE_SOULBURN] = self;
  }
}

static void soulburn_on_update(Effect *effect, double dt) {
  Soulburn *self = (Soulburn *)effect;
  AffectableComponent *affectable = self->base_effect.container;

  float amt = affectable_component_damage(affectable, dt * self->dps);
  effect->amount_affected += amt;

  AffectableComponent *user_affectable = &((Player *)self->user)->affectable;
  float heal_amt =
      affectable_component_heal(user_affectable, self->leech_pct * amt);

  particle_system_update((GameObject *)self->particle_system, dt);
}

static void soulburn_on_render(Effect *effect, Renderer *renderer) {
  Soulburn *self = (Soulburn *)effect;
  particle_system_render((GameObject *)self->particle_system, renderer);
}

static void soulburn_on_free(Effect *effect) {
  Soulburn *self = (Soulburn *)effect;
  particle_system_free((GameObject *)self->particle_system);
  free(self);
}
