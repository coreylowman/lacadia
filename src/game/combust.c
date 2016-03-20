#include "enemies/enemy.h"
#include "affectable_object.h"
#include "effect.h"
#include "combust.h"
#include "ability.h"
#include "players/player.h"

Ability combust_ability = {
    .cooldown = 0,
    .max_cooldown = 0,
    .on_use = combust_use
};

static void combust_apply(GameWorld *world, Enemy *enemy){
    int i = affectable_object_index_of_effect(&enemy->affectable, EFFECT_TYPE_BURN);
    if(i != -1){
        Effect *effect = enemy->affectable.effects->data[i];
        //update with the remainder of the duration
        effect->on_update(effect, &enemy->affectable, effect->duration);
        effect->duration = 0.0;
    }
}

void combust_use(GameWorld *world, GameObject *user){
    Player *player = user;
    float radius = 10;
    game_world_apply_to_enemies(world, player->collidable.bounding_box.center, 10, combust_apply);
}
