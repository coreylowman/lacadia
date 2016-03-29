#include <stdlib.h>
#include "mage.h"
#include "abilities/spell.h"
#include "abilities/ability.h"
#include "abilities/fireball.h"
#include "abilities/wildfire.h"
#include "abilities/combust.h"
#include "abilities/icicle.h"
#include "enemies/enemy.h"

extern Ability fireball_ability;
extern Ability wildfire_ability;
extern Ability combust_ability;

extern Ability icicle_ability;

Player *mage_new(GameWorld *world){
    Player *self = player_new(world);
    //todo init abilities
    self->abilities[0] = icicle_ability;
    self->abilities[1] = wildfire_ability;
    self->abilities[2] = combust_ability;
    // self->abilities[3] = dragons_breath_ability;

    self->passive = mage_passive;

    self->affectable.stats.max_health = 100;
    affectable_object_init(&self->affectable);

    self->moveable.speed = 5.0;
    self->moveable.direction = (Vec3) { .data = { 0, 0, -1 } };
    self->moveable.position = (Vec3) { .data = { 0, 0, 0 } };

    self->renderable.model_id = game_world_get_model_id(world, "assets/mage");
    renderable_object_update(&self->renderable, self->moveable);

    self->collidable.container = self;
    self->collidable.is_colliding = collidable_object_is_colliding;
    self->collidable.on_collide = player_on_collide;
    self->collidable.bounding_box = game_world_get_model_obb(world, self->renderable.model_id);
    collidable_object_update(&self->collidable, self->moveable);
    
    self->stance = 0;
    self->on_switch_stance = mage_on_switch_stance;

    return self;
}

//0 => fire stance
//1 => ice stance
void mage_on_switch_stance(Player *self){
    self->stance = !self->stance;
    if(self->stance){
        // self->abilities[0] = icicle_ability;
        // self->abilities[1] = frost_nova_ability;
        // self->abilities[3] = ice_storm_ability;
    }else{
        // self->abilities[0] = fireball_ability;
        // self->abilities[1] = spread_burn_ability;
        // self->abilities[3] = dragons_breath_ability;
    }
}

static double this_dt;

static void passive_tick(GameWorld *world, Enemy *enemy){
    affectable_object_damage(&enemy->affectable, this_dt * 0.25f);
}

void mage_passive(Player *self, double dt){
	this_dt = dt;
    float radius = 5.0f;
    game_world_apply_to_enemies(self->base_object.world, self->moveable.position, radius, passive_tick);
}
