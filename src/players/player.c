#include <stdio.h>
#include <stdlib.h>
#include "player.h"

Player *player_new(GameWorld *world){
    Player *self = malloc(sizeof(*self));
    self->base_object = game_object_new(world, GAME_OBJECT_TYPE_PLAYER);
    self->affectable.effects = set_new(effect_free);
    return self;
}

void player_free(Player *self){
    game_object_free(self->base_object);
    set_free(self->affectable.effects);
    free(self);
}

void player_update(Player *self, double dt){
    affectable_object_update(&self->affectable, dt);
    moveable_object_update(&self->moveable, dt);
    //TODO update renderable.model_matrix
}

void player_use_ability(Player *self, int i){
    self->abilities[i].on_use(self->base_object->world, self);
}
