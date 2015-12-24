#ifndef EFFECT_H
#define EFFECT_H

#include "util/game_object.h"

typedef enum {
    BURN,
    SOULBURN,
    FROST,
    PERMAFROST,
} EffectType;

typedef struct Effect
{
    EffectType type;
    
    float amt;
    float duration;

    void (*apply)(struct Effect *self, GameObject *obj, double dt);
    void (*update)(struct Effect *self, double dt);
    void (*on_end)(struct Effect *self, GameObject *obj, double dt);
} Effect;

Effect *effect_new();
void effect_free(Effect *self);

#endif
