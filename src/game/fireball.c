#include <stdio.h>
#include <stdlib.h>
#include "players/player.h"
#include "fireball.h"
#include "enemies/enemy.h"
#include "util/set.h"

Ability fireball_ability = {
    .cooldown = 0,
    .max_cooldown = 0,
    .on_use = fireball_use
};

void fireball_use(GameWorld *world, GameObject *user){
    game_world_add_spell(world, fireball_new(world, user));
}

Spell *fireball_new(GameWorld *world, GameObject *user){
    Spell *self = spell_new(world);

    if(user->type == GAME_OBJECT_TYPE_PLAYER){
        Player *player = user->container;
		self->moveable = player->moveable;
        self->moveable.position = vec3_add(player->moveable.position, player->moveable.direction);
		self->moveable.position.y += player->collidable.bounding_box.radius.y;
    }

    self->renderable.asset_id = game_world_get_asset_id(world, "assets/fireball");
    renderable_object_update(&self->renderable, self->moveable);

    self->collidable.self = self->base_object;
    self->collidable.on_collide = fireball_on_collide;
    self->collidable.bounding_box = game_world_get_asset_obb(world, self->renderable.asset_id);
    collidable_object_update(&self->collidable, self->moveable);

    self->on_update = fireball_update;

    return self;
}

void fireball_update(Spell *self, double dt){
    moveable_object_update(&self->moveable, dt);
    renderable_object_update(&self->renderable, self->moveable);
    collidable_object_update(&self->collidable, self->moveable);
}

void fireball_on_collide(GameObject *self, GameObject *other){
    if(other->type == GAME_OBJECT_TYPE_ENEMY){
        Enemy *enemy = other->container;
        enemy_damage(enemy, 1);
		affectable_object_affect(&enemy->affectable, burn_new(1, 4));
    }
    self->destroy = 1;
}

typedef struct {
    int degree;
    float dps;
} BurnData;

Effect *burn_new(float dmg, float duration){
    Effect *self = effect_new(EFFECT_TYPE_BURN, duration);
    
    self->data = malloc(sizeof(BurnData));
	BurnData *data = self->data;
	data->degree = 1;
	data->dps = dmg;
    
    self->on_apply = burn_on_apply;
    self->on_update = burn_on_update;
    self->on_end = burn_on_end;
    self->on_free = burn_on_free;
    self->is_over = burn_is_over;

    return self;
}

void burn_on_apply(Effect *self, AffectableObject *affectable){
	int i;
	Effect *e;
	for (i = 0; i < affectable->effects->length; i++) {
		if (affectable->effects->data[i] == NULL) continue;
		e = affectable->effects->data[i];
		if (e->type == EFFECT_TYPE_BURN) {
			e->duration = e->max_duration;
			BurnData *data = e->data;
			data->degree = min(data->degree + 1, 3);
			effect_free(self);
			return;
		}
	}
    set_add(affectable->effects, self);
}

void burn_on_update(Effect *self, AffectableObject *affectable, double dt){
    BurnData *data = self->data;
    affectable->stats.health -= dt * data->degree * data->dps;
    self->duration -= dt;
}

void burn_on_end(Effect *self, AffectableObject *affectable){
    int i;
    for(i = 0;i < affectable->effects->length;i++){
        if(affectable->effects->data[i] == NULL) continue;
        if(affectable->effects->data[i] == self){
            set_remove_at(affectable->effects, i);
            return;
        }
    }
}

void burn_on_free(Effect *self){
    free(self->data);
    free(self);
}

int burn_is_over(Effect *self){
    return self->duration <= 0.0;
}

