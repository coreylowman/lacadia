#ifndef EFFECT_H
#define EFFECT_H

#include "util/game_object.h"

typedef enum {
    BURN,
    SOULBURN,
    FROST,
    PERMAFROST,
} EffectType;

//effects are applied by some spells
//e.g. fireball applies a burn effect that deals some damage over time
typedef struct Effect{
    //type... so you know what kinds of effects the object is under... could
    // have spells that need to know that kinda stuff
    EffectType type;
    
    //generic amount... for damage effects it could be the amount of damage per
    //tick, for stat affects it could be a percentage, etc.
    float amt;
    
    //how long it lasts
    float duration;

    //generic object methods
    //apply -> update -> ... -> update -> on_end
    void (*apply)(struct Effect *self, GameObject *obj, double dt);
    void (*update)(struct Effect *self, double dt);
    void (*on_end)(struct Effect *self, GameObject *obj, double dt);
} Effect;

Effect *effect_new();
void effect_free(Effect *self);

#endif
