#include <stdlib.h>
#include "lacadia/enemies/enemy.h"
#include "lacadia/effects/affectable_component.h"
#include "lacadia/effects/effect.h"
#include "ability.h"
#include "lacadia/players/player.h"

static void combust_apply(GameWorld *world, GameObject *user,
                          GameObject *target) {
  Enemy *enemy = (Enemy *)target;
  // update burns with the remainder of the duration
  Effect *effect = enemy->affectable->effects[EFFECT_TYPE_BURN];
  if (effect != NULL) {
    effect_advance(effect, effect->duration);
  }

  effect = enemy->affectable->effects[EFFECT_TYPE_SOULBURN];
  if (effect != NULL) {
    effect_advance(effect, effect->duration);
  }
}

void combust_use(GameWorld *world, GameObject *user) {
  float radius = 10;
  game_world_apply(world, "enemy", user, 10, combust_apply);
}

Ability combust_ability = {
    .cooldown = 0, .max_cooldown = 7, .on_use = combust_use};