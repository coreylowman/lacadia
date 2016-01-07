#ifndef EFFECT_H
#define EFFECT_H

#include "game/affectable_object.h"

typedef enum {
    SHIELD,
    SLOW,
    STUN, //just need to make methods... no separate data req.
    DOT,
    BURN, 
    SOULBURN, //just use a doteffect... just a typed dot
    FROST,
    PERMAFROST, //just use a sloweffect... just a typed slow
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
    float max_duration;

    //generic object methods
    //apply -> update -> ... -> update -> on_end
    void (*on_apply)(struct Effect *self, struct AffectableObject *obj, double dt);
    void (*on_update)(struct Effect *self, struct AffectableObject *obj, double dt);
    void (*on_end)(struct Effect *self, struct AffectableObject *obj, double dt);
    void (*is_over)(struct Effect *self);
} Effect;

Effect *effect_new();
void effect_free(Effect *self);

#endif
