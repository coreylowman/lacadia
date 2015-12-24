#include "speed.h"
#include "util/game_object.h"
#include "players/player.h"

static void speed_apply(Effect *self, GameObject *obj, double dt){
    if(obj->type == GAME_OBJECT_TYPE_PLAYER){
        Player *player = (Player *)obj;
        player->stats.speed = self->amt * player->stats.max_speed;
    }
}
 
static void speed_update(Effect *self, double dt){
    self->duration -= dt;
}
 
static void speed_on_end(Effect *self, GameObject *obj, double dt){
    if(obj->type == GAME_OBJECT_TYPE_PLAYER){
        Player *player = (Player *)obj;
        player->stats.speed = player->stats.max_speed;
    }
}

Effect *speed_new(float speed_amt, float duration){
    Effect *self = effect_new();

    self->amt = speed_amt;
    self->duration = duration;

    self->apply = speed_apply;
    self->update = speed_update;
    self->on_end = speed_on_end;

    return self;
}
