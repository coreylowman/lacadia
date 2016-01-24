#include <stdlib.h>
#include "effect.h"

Effect *effect_new(EffectType type, float duration){
    Effect *self = malloc(sizeof(*self));
    self->type = type;
    self->duration = duration;
	self->max_duration = duration;
    return self;
}

void effect_free(Effect *self){
    self->on_free(self);
}

