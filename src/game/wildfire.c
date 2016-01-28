#include "wildfire.h"
#include "affectable_object.h"
#include "effect.h"
#include "fireball.h"

static void wildfire_apply(GameWorld *world, Enemy *enemy){
    int i;
    Effect *e;
    BurnData *data;
    for(i = 0;i < enemy->affectable.effects->length;i++){
        if(enemy->affectable.effects->data[i] == NULL) continue;
        e = enemy->affectable.effects->data[i];

        if(e->type == EFFECT_TYPE_BURN){
            data = e->data;
            data->degree = min(3, data->degree);
            e->duration = e->max_duration;
            game_world_apply_to_enemies(world, enemy->collidable.bounding_box.center, 10, wildfire_spread);
            return;
        }
    }
}

static void wildfire_spread(GameWorld *world, Enemy *enemy){
    int i;
    Effect *e;
    BurnData *data;
    for(i = 0;i < enemy->affectable.effects->length;i++){
        if(enemy->affectable.effects->data[i] == NULL) continue;
        e = enemy->affectable.effects->data[i];

        if(e->type == EFFECT_TYPE_BURN){
            data = e->data;
            data->degree = min(3, data->degree);
            e->duration = e->max_duration;
            return;
        }
    }

    affectable_object_affect(&enemy->affectable, burn_new(world, &enemy->moveable, 1, 4));
}

void wildfire_use(GameWorld *world, GameObject *user){
    Player *player = user->container;
    float radius = 10;
    game_world_apply_to_enemies(world, player->collidable.bounding_box.center, 10, wildfire_apply);
}

