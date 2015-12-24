#include <stdlib.h>
#include <stdio.h>
#include "teleport.h"
#include "util/array_list.h"

Ability teleport_ability = 
{ 
    .max_cooldown = 1.5,
    .cooldown = 0,
    .on_use = teleport_use
};

void teleport_use(GameWorld *world, GameObject *user){

}
