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
	self->base_object.position = VEC3_ZERO;
	self->base_object.direction = (Vec3) { .data = { 0, 0, -1 } };
    //todo init abilities
    self->abilities[0] = fireball_ability;
    self->abilities[1] = wildfire_ability;
    self->abilities[2] = combust_ability;
    // self->abilities[3] = dragons_breath_ability;

    self->passive = mage_passive;

	self->affectable = affectable_component_init(&self->base_object, 100, 5.0, 0, 0, 0);
	self->renderable = renderable_component_init(&self->base_object, "assets/mage", world->renderer);
	self->collidable = collidable_component_init(&self->base_object, game_world_get_model_obb(world, self->renderable.model_id), player_on_collide);
    
    self->stance = 0;
    self->on_switch_stance = mage_on_switch_stance;

    return self;
}

//0 => fire stance
//1 => ice stance
void mage_on_switch_stance(Player *self){
    self->stance = !self->stance;
    if(self->stance){
        self->abilities[0] = icicle_ability;
        // self->abilities[1] = frost_nova_ability;
        // self->abilities[3] = ice_storm_ability;
    }else{
        self->abilities[0] = fireball_ability;
        // self->abilities[1] = spread_burn_ability;
        // self->abilities[3] = dragons_breath_ability;
    }
}

// TODO change apply_to_enemies functions to take a dt param
static double this_dt;

static void passive_tick(GameWorld *world, GameObject *user, Enemy *enemy){
    affectable_component_damage(&enemy->affectable, this_dt * 0.25f);
}

void mage_passive(Player *self, double dt){
	this_dt = dt;
    float radius = 5.0f;
    game_world_apply_to_enemies(self->base_object.world, self, radius, passive_tick);
}
