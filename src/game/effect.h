#ifndef EFFECT_H
#define EFFECT_H

#include "game/affectable_object.h"
#include "game/game_world.h"

typedef enum {
    EFFECT_TYPE_SHIELD,
    EFFECT_TYPE_SLOW,
    EFFECT_TYPE_STUN, //just need to make methods... no separate data req.
    EFFECT_TYPE_DOT,
    EFFECT_TYPE_BURN, 
    EFFECT_TYPE_SOULBURN, //just use a doteffect... just a typed dot
    EFFECT_TYPE_FROST,
    EFFECT_TYPE_PERMAFROST, //just use a sloweffect... just a typed slow
} EffectType;

//effects are applied by some spells
//e.g. fireball applies a burn effect that deals some damage over time
typedef struct Effect{
    //type... so you know what kinds of effects the object is under... could
    // have spells that need to know that kinda stuff
    EffectType type;
    
    //generic effect data
    void *data;
    
    //how long it lasts
    float duration;
    float max_duration;

    //generic object methods
    //apply -> update -> ... -> update -> on_end
    void (*on_apply)(struct Effect *self, struct AffectableObject *obj);
    void (*on_update)(struct Effect *self, struct AffectableObject *obj, double dt);
    void (*on_render)(struct Effect *self, GameWorld *world);
    void (*on_end)(struct Effect *self, struct AffectableObject *obj);
    void (*on_free)(struct Effect *self);
    int (*is_over)(struct Effect *self);
} Effect;

Effect *effect_new(EffectType type, float duration);
void effect_free(Effect *self);

#endif
