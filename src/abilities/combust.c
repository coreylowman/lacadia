#include <stdlib.h>
#include "enemies/enemy.h"
#include "components/affectable_component.h"
#include "effect.h"
#include "combust.h"
#include "ability.h"
#include "players/player.h"

Ability combust_ability = {
    .cooldown = 0,
    .max_cooldown = 0,
    .on_use = combust_use
};

static void combust_apply(GameWorld *world, GameObject *user, Enemy *enemy){
    Effect *effect = enemy->affectable.effects[EFFECT_TYPE_BURN];
	if (effect != NULL) {
		//update with the remainder of the duration
		effect_update(effect, effect->duration);
	}
}

void combust_use(GameWorld *world, GameObject *user){
    float radius = 10;
    game_world_apply_to_enemies(world, user, 10, combust_apply);
}
