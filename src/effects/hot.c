#include "hot.h"
#include "util/game_object.h"
#include "players/player.h"

static void hot_apply(Effect *self, GameObject *obj, double dt){
    if(obj->type == GAME_OBJECT_TYPE_PLAYER){
        Player *player = (Player *)obj;
        player->stats.health += dt * self->amt;
    }
}
 
static void hot_update(Effect *self, double dt){
    self->duration -= dt;
}

Effect *hot_new(float tick_amt, float duration){
    Effect *self = effect_new();

    self->amt = tick_amt;
    self->duration = duration;

    self->apply = hot_apply;
    self->update = hot_update;
    self->on_end = NULL;

    return self;
}
