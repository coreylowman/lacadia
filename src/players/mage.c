#include <stdlib.h>
#include "mage.h"

//ability 1
// extern Ability fireball_ability; //burns target - DoT
// extern Ability icicle_ability; //slows target

// //ability 2
// //targets affected that are hit by icicle, will bounce icicle to another target
// //when it
// extern Ability frost_nova_ability;
// //every enemy in certain distance with a burn, spreads that burn dot to 1 nearby
// //enemy
// extern Ability spread_burn_ability;

// //ability 3
// extern Ability teleport_ability;

// //ability 4
// //targets wherever you click in a radius around it
// extern Ability ice_storm_ability;
// //burned targets have the DoT removed, but it deals the remaining damage instantly
// //targets in a cone in the direction you are facing
// extern Ability dragons_breath_ability;

Player *mage_new(GameWorld *world){
    Player *self = player_new(world);
    //todo init abilities
    // self->abilities[0] = fireball_ability;
    // self->abilities[1] = spread_burn_ability;
    // self->abilities[2] = teleport_ability;
    // self->abilities[3] = dragons_breath_ability;

    //todo set up stats in affectable

    //todo set bounding box in collidable
    //todo create collidable?
    //todo set position and speed in moveable

    //todo model_matrix in renderable
    self->renderable.asset_id = game_world_get_asset_id(world, "assets/lacadia_mage");
    mat4_ident(&self->renderable.model_matrix);
    
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
