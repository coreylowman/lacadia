#include <stdlib.h>
#include "effect.h"

Effect *effect_new(){
    Effect *self = malloc(sizeof(*self));
    self->amt = 0;
    self->duration = 0;
    return self;
}

void effect_free(Effect *self){
    free(self);
}

