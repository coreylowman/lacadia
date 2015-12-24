#include <stdlib.h>
#include "ability.h"

Ability *ability_new(){
    Ability *self = malloc(sizeof(*self));
    self->max_cooldown = 0;
    self->cooldown = 0;
    return self;
}

void ability_free(Ability *self){
    free(self);
}
