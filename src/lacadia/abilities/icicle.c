#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lacadia/players/player.h"
#include "lacadia/enemies/enemy.h"
#include "engine/particle_system.h"
#include "spell.h"
#include "engine/util/random.h"
#include "lacadia/effects/frost.h"

static Spell *icicle_new(GameWorld *world, GameObject *user);
static void on_collide(GameObject *self, GameObject *other);

void icicle_use(GameWorld *world, GameObject *user) {
  game_world_add_object(world, (GameObject *)icicle_new(world, user));
}

static Spell *icicle_new(GameWorld *world, GameObject *user) {
  Spell *self = spell_new(world, user, "icicle", on_collide);

  self->base_object.position = vec3_add(user->position, user->direction);
  self->base_object.position.y += 2;
  self->base_object.direction = user->direction;
  self->speed = 30.0;

  self->target = NULL;

  return self;
}

static void fizzle_particle_init(Particle *p, Vec3 position, float duration) {
  p->position = position;
  p->velocity = random_length_vec3(3.0);
  p->duration = random_in_rangef(0, duration);
}

static void on_collide(GameObject *self, GameObject *other) {
  if (strcmp(other->tag, "enemy") == 0) {
    Enemy *enemy = (Enemy *)other;
    affectable_component_damage(enemy->affectable, 1);
    affectable_component_affect(
        enemy->affectable,
        (Effect *)frost_new(self->world, &enemy->base_object, 0.1, 4));
    self->destroy = 1;
  } else if (strcmp(other->tag, "wall") == 0) {
    Spell *icicle = (Spell *)self;
    ParticleSystem *ps = particle_system_new(
        self->world, icicle->collidable->bounding_box.center,
        "frost_particle", 32, 0.0, 0.75);
    particle_system_set_particle_init(ps, fizzle_particle_init);
    // this gives ownership to game_world... we don't have to worry about
    // freeing
    game_world_add_object(self->world, (GameObject *)ps);
    self->destroy = 1;
  }
}

Ability icicle_ability = {
    .cooldown = 0, .max_cooldown = 0.33, .on_use = icicle_use};
