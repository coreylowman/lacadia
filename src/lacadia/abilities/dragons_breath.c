#include <stdio.h>
#include <stdlib.h>
#include "lacadia/players/player.h"
#include "lacadia/enemies/enemy.h"
#include "engine/util/set.h"
#include "engine/particle_system.h"
#include "lacadia/effects/soulburn.h"
#include "spell.h"
#include "engine/util/random.h"

static void dragons_breath_apply(GameWorld *world, GameObject *user,
                                 GameObject *target) {
  Enemy *enemy = (Enemy *)target;
  Vec3 to_enemy = vec3_sub(enemy->base_object.position, user->position);
  float degrees_between = vec3_degrees_between(user->direction, to_enemy);
  if (degrees_between <= 45) {
    affectable_component_damage(enemy->affectable, 2);
    affectable_component_affect(
        enemy->affectable,
        (Effect *)soulburn_new(world, &enemy->base_object, user, 3, 0.5, 1.5));
  }
}

static Vec3 direction;

static void particle_init(Particle *p, Vec3 position, float duration) {
  float t = random_in_rangef(1.0f, 3.0f);
  p->position = position;
  Vec3 pdirection = vec3_scale(direction, t * 10.0f / duration);
  p->velocity = vec3_rotate_around_y(pdirection, random_in_rangef(-22.5, 22.5));
  p->duration = duration / t;
}

void dragons_breath_use(GameWorld *world, GameObject *user) {
  float radius = 10.0f;

  direction = user->direction;
  direction.y = 0;
  vec3_normalize(&direction);
  game_world_apply(world, "enemy", user, radius, dragons_breath_apply);

  ParticleSystem *ps = particle_system_new(
      world, user->position, "assets/burn_particle", 64, 0.0, 0.75);
  particle_system_set_particle_init(ps, particle_init);
  particle_system_set_scale_over_duration(ps, 0);
  game_world_add_object(world, (GameObject *)ps);
}

Ability dragons_breath_ability = {
    .cooldown = 0, .max_cooldown = 3, .on_use = dragons_breath_use};
