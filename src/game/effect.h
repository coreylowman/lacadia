#ifndef EFFECT_H
#define EFFECT_H

#include "game/affectable_object.h"
#include "game/game_world.h"

typedef enum EffectType {
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
    //apply -> update -> ... -> update -> is_over? -> on_end

    //called when the effect is first applied to an object... usually when a spell
    //collides with an enemy
    void (*on_apply)(struct Effect *self, struct AffectableObject *obj);

    //called by the affectable object when it updates itself
    void (*on_update)(struct Effect *self, struct AffectableObject *obj, double dt);
    
    //called by affectable_object_update to see whether this effect has ended
    //it then calls on_end
    int  (*is_over)(struct Effect *self);

    //called when an effect ends, this handles removing itself from the
    //affectable object, which may call the on_free method
    void (*on_end)(struct Effect *self, struct AffectableObject *obj);
    
    //called by effect_free
    void (*on_free)(struct Effect *self);

    //renders the effect
    void (*on_render)(struct Effect *self, GameWorld *world);
} Effect;

Effect *effect_new(EffectType type, float duration);
void effect_free(Effect *self);

#endif
