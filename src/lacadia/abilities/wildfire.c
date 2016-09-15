#include "lacadia/effects/affectable_component.h"
#include "lacadia/effects/effect.h"
#include "lacadia/effects/burn.h"
#include "lacadia/enemies/enemy.h"
#include "lacadia/players/player.h"
#include "ability.h"
#include "spell.h"
#include "engine/util/vec3.h"

static Spell *wildfire_new(GameWorld *world, GameObject *user,
                           GameObject *target);
static Spell *wildfire_spread_new(GameWorld *world, GameObject *user,
                                  GameObject *target);
static void wildfire_update(GameObject *self, double dt);
static void wildfire_on_collide(GameObject *self, GameObject *other);
static void wildfire_spread_on_collide(GameObject *self, GameObject *other);
static void wildfire_apply(GameWorld *world, GameObject *user,
                           GameObject *enemy);
static void wildfire_spread(GameWorld *world, GameObject *origin,
                            GameObject *enemy);

static Spell *wildfire_new(GameWorld *world, GameObject *user,
                           GameObject *target) {
  Spell *self = spell_new(world, wildfire_update, spell_render, spell_free);

  self->base_object.position = user->position;
  self->base_object.position.y += 5;
  self->base_object.direction = user->direction;
  self->speed = 50.0;

  game_object_alloc_components(&self->base_object, 2);
  self->renderable = renderable_component_new(
      &self->base_object, "assets/burn_particle", world->renderer);
  self->collidable = collidable_component_new(
      &self->base_object,
      game_world_get_model_obb(world, self->renderable->model_id),
      wildfire_on_collide);
  self->collidable->is_colliding = spell_is_colliding_with_target;
  self->base_object.components[0] = (Component *)self->renderable;
  self->base_object.components[1] = (Component *)self->collidable;

  self->target = target;

  return self;
}

// this is used once the regular wildfire hits an enemy, it then spreads it to
// other enemies
// around the initial target
static Spell *wildfire_spread_new(GameWorld *world, GameObject *user,
                                  GameObject *target) {
  Spell *self = spell_new(world, wildfire_update, spell_render, spell_free);

  self->base_object.position = user->position;
  self->base_object.position.y += 5;
  self->base_object.direction = user->direction;
  self->speed = 50.0;

  game_object_alloc_components(&self->base_object, 2);
  self->renderable = renderable_component_new(
      &self->base_object, "assets/burn_particle", world->renderer);
  self->collidable = collidable_component_new(
      &self->base_object,
      game_world_get_model_obb(world, self->renderable->model_id),
      wildfire_spread_on_collide);
  self->collidable->is_colliding = spell_is_colliding_with_target;
  self->base_object.components[0] = (Component *)self->renderable;
  self->base_object.components[1] = (Component *)self->collidable;

  self->target = target;

  return self;
}

static void wildfire_update(GameObject *obj, double dt) {
  Spell *self = (Spell *)obj;
  if (self->target->destroy) {
    self->base_object.destroy = 1;
    return;
  }
  spell_update(obj, dt);

  Vec3 pos = self->base_object.position;
  Vec3 target_pos = ((Enemy *)(self->target))->base_object.position;
  self->base_object.direction = vec3_sub(target_pos, pos);
  vec3_normalize(&self->base_object.direction);
}

static void wildfire_on_collide(GameObject *self, GameObject *other) {
  Enemy *enemy = (Enemy *)other;

  if (enemy->affectable->effects[EFFECT_TYPE_BURN] != NULL) {
    // was fired at enemies who are already affected by burn
    burn_increase_degree(enemy->affectable->effects[EFFECT_TYPE_BURN]);
  } else {
    affectable_component_affect(
        enemy->affectable,
        (Effect *)burn_new(self->world, &enemy->base_object, 1, 4));
  }

  game_world_apply(self->world, "enemy", other, 10, wildfire_spread);

  self->destroy = 1;
}

static void wildfire_spread_on_collide(GameObject *self, GameObject *other) {
  Enemy *enemy = (Enemy *)other;
  // have to check for a burn again, even though we only fired at ones
  // without a burn on them, this spell could fire multiple spreads at the
  // same target. so in the time it took this one to collide, another one
  // could've already applied
  // if it doesn't have a burn, then apply one!
  if (enemy->affectable->effects[EFFECT_TYPE_BURN] == NULL) {
    affectable_component_affect(
        enemy->affectable,
        (Effect *)burn_new(self->world, &enemy->base_object, 1, 4));
  }
  self->destroy = 1;
}

static void wildfire_spread(GameWorld *world, GameObject *origin,
                            GameObject *target) {
  Enemy *enemy = (Enemy *)target;
  if (enemy->affectable->effects[EFFECT_TYPE_BURN] == NULL) {
    // if it doesn't have a burn on it, then shoot a wildfire spread at it
    game_world_add_object(world, (GameObject *)wildfire_spread_new(
                                     world, origin, (GameObject *)enemy));
  }
}

static void wildfire_apply(GameWorld *world, GameObject *user,
                           GameObject *target) {
  Enemy *enemy = (Enemy *)target;
  if (enemy->affectable->effects[EFFECT_TYPE_BURN] != NULL) {
    // if it already has a burn on it shoot a wildfire at it
    game_world_add_object(
        world, (GameObject *)wildfire_new(world, user, (GameObject *)enemy));
  }
}

void wildfire_use(GameWorld *world, GameObject *user) {
  float radius = 10;
  game_world_apply(world, "enemy", user, radius, wildfire_apply);
}

Ability wildfire_ability = {
    .cooldown = 0, .max_cooldown = 1, .on_use = wildfire_use};
