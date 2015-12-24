#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "effects/effect.h"

Player *player_new(GameWorld *world){
    Player *self = malloc(sizeof(*self));
    self->base_object = game_object_new(world, GAME_OBJECT_TYPE_PLAYER);
    self->effects = array_list_new(effect_free);

    mat4_ident(&self->model_matrix);

    return self;
}

void player_free(Player *self){
    game_object_free(self->base_object);
    array_list_free(self->effects);
    free(self);
}

void player_update(Player *self, double dt){
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

void player_render(Player *self){
	game_world_draw_asset(self->base_object->world, self->asset_id, self->model_matrix);
}

void player_use_ability(Player *self, int i){
    self->abilities[i].on_use(self->base_object->world, self);
}
