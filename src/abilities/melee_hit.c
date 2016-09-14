#include "players/player.h"
#include "enemies/enemy.h"
#include "ability.h"

void melee_hit_use(GameWorld *world, GameObject *user) {
  Enemy *enemy = (Enemy *)user;
  Player *player = (Player *)enemy->target;
  affectable_component_damage(&player->affectable, 1 * enemy->affectable.power);
}

Ability melee_hit_ability = {
    .cooldown = 0, .max_cooldown = 0, .on_use = melee_hit_use};
