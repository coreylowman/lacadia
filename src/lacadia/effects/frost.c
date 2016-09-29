#include <stdlib.h>
#include "frost.h"
#include "engine/util/random.h"
#include "engine/graphics/renderer.h"
#include "lacadia/enemies/enemy.h"
#include "lacadia/effects/affectable_component.h"
#include "permafrost.h"

static void frost_on_apply(Effect *self, AffectableComponent *affectable);
static void frost_on_update(Effect *self, double dt);
static void frost_on_render(Effect *self, Renderer *renderer);
static void frost_on_end(Effect *self);
static void frost_on_free(Effect *self);

static void frost_particle_init(Particle *p, Vec3 position, float duration) {
  p->position = vec3_add(position, random_length_vec3(1.0));
  p->velocity = (Vec3){.data = {0, -1, 0}};
  p->duration = random_in_rangef(0, duration);
}

Frost *frost_new(GameWorld *world, GameObject *target,
                 float slow_pct_per_degree, float duration) {
  Frost *self = malloc(sizeof(*self));

  self->base_effect = effect_init(EFFECT_TYPE_FROST, duration);

  self->degree = 0;
  self->slow_pct_per_degree = slow_pct_per_degree;

  self->particle_system = particle_system_new(
      world, target->position, "frost_particle", 8, duration, duration * 0.4);
  particle_system_set_particle_init(self->particle_system, frost_particle_init);
  particle_system_set_follow_target(self->particle_system, target);

  self->base_effect.on_apply = frost_on_apply;
  self->base_effect.on_update = frost_on_update;
  self->base_effect.on_render = frost_on_render;
  self->base_effect.on_end = frost_on_end;
  self->base_effect.on_free = frost_on_free;

  return self;
}

void frost_increase_degree(Effect *effect) {
  Frost *self = (Frost *)effect;

  effect->duration = effect->max_duration;
  self->particle_system->duration = effect->duration;

  particle_system_double_particles(self->particle_system);
  self->degree = self->degree + 1;

  // slow here
  float amt = self->slow_pct_per_degree * self->base_effect.container->speed;
  effect->amount_affected +=
      affectable_component_slow(self->base_effect.container, amt);
}

static void fizzle_particle_init(Particle *p, Vec3 position, float duration) {
  p->position = position;
  p->velocity = random_length_vec3(7.0);
  p->duration = random_in_rangef(0, duration);
}

static void frost_on_apply(Effect *self, AffectableComponent *affectable) {
  Frost *frost = (Frost *)affectable->effects[EFFECT_TYPE_FROST];
  if (frost != NULL) {
    // if frost is already on the target, try to increase the degree of the
    // exisiting one
    if (frost->degree == 3) {
      // remove current frost from affectable
      affectable_component_remove(affectable, EFFECT_TYPE_FROST);

      affectable_component_affect(
          affectable, (Effect *)permafrost_new(
                          affectable->base_component.container->world,
                          affectable->base_component.container, 4, .5, 2.0));

      // show burst of particles indicating permafrost has hit
      Enemy *enemy = (Enemy *)affectable->base_component.container;
      ParticleSystem *ps = particle_system_new(
          enemy->base_object.world, enemy->collidable->bounding_box.center,
          "./assets/frost_particle", 64, 0.0, 0.75);
      particle_system_set_particle_init(ps, fizzle_particle_init);
      game_world_add_object(enemy->base_object.world, (GameObject *)ps);
    } else {
      frost_increase_degree((Effect *)frost);
    }

    // free the effect we were trying to add, since we just modified the frost
    // already on the target
    effect_free(self);
    return;
  } else {
    // there is no frost already on the target... add it
    affectable->effects[EFFECT_TYPE_FROST] = self;
    frost_increase_degree((Effect *)self);
  }
}

static void frost_on_update(Effect *effect, double dt) {
  Frost *self = (Frost *)effect;

  particle_system_update((GameObject *)self->particle_system, dt);
}

static void frost_on_render(Effect *effect, Renderer *renderer) {
  Frost *self = (Frost *)effect;
  particle_system_render((GameObject *)self->particle_system, renderer);
}

static void frost_on_end(Effect *self) {
  // undo the slow
  self->amount_affected -=
      affectable_component_quicken(self->container, self->amount_affected);
}

static void frost_on_free(Effect *effect) {
  Frost *self = (Frost *)effect;
  particle_system_free((GameObject *)self->particle_system);
  free(self);
}
