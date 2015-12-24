#include <stdio.h>
#include <stdlib.h>
#include "enemy.h"

Enemy *enemy_new(GameWorld *world){
    Enemy *self = malloc(sizeof(*self));
    self->base_object = game_object_new(world, GAME_OBJECT_TYPE_ENEMY);
	self->effects = array_list_new(effect_free);
    return self;
}

void enemy_free(Enemy *self){
    self->target = NULL;
    game_object_free(self->base_object);
	array_list_free(self->effects);
    free(self);
}

void enemy_update(Enemy *self, double dt){
    game_object_update(self->base_object, dt);

    int i;
    Effect *e;
    for(i = 0;i < self->effects->length;i++){
        e = self->effects->data[i];
        e->update(e, dt);
        if(e->duration <= 0 && e->on_end != NULL){
            e->on_end(e, self->base_object, dt);
        }else{
            e->apply(e, self->base_object, dt);
        }
    }
    for(i = self->effects->length - 1;i >= 0;i--){
        e = self->effects->data[i];
        if(e->duration <= 0){
            array_list_remove_at(self->effects, i);
        }
    }
}

void enemy_render(Enemy *self){
    
}
