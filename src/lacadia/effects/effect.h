#ifndef EFFECT_H
#define EFFECT_H

#include "engine/graphics/renderer.h"

typedef struct AffectableComponent AffectableComponent;

typedef enum EffectType {
  EFFECT_TYPE_SHIELD,
  EFFECT_TYPE_SLOW,
  EFFECT_TYPE_STUN, // just need to make methods... no separate data req.
  EFFECT_TYPE_DOT,
  EFFECT_TYPE_BURN,
  EFFECT_TYPE_SOULBURN, // just use a doteffect... just a typed dot
  EFFECT_TYPE_FROST,
  EFFECT_TYPE_PERMAFROST, // just use a sloweffect... just a typed slow
  EFFECT_TYPE_MAX,
} EffectType;

// effects are applied by some spells
// e.g. fireball applies a burn effect that deals some damage over time
typedef struct Effect {
  // type... so you know what kinds of effects the object is under... could
  // have spells that need to know that kinda stuff
  EffectType type;

  // how long it lasts
  float duration;
  float max_duration;

  // the affectable component this effect lives in
  // set in effect_apply
  AffectableComponent *container;

  // the amoun that the effect has changed the stat
  // e.g. if a slow effect has slowed a monster with 10 speed by 10%,
  // this value would be 1. then when the affect is over, this value
  // would be readded to the speed
  float amount_affected;

  // called when the effect is first applied to an object... usually when a
  // spell
  // collides with an enemy
  void (*on_apply)(struct Effect *self, AffectableComponent *obj);

  // called by the affectable object when it updates itself
  void (*on_update)(struct Effect *self, double dt);

  // called when an effect ends
  void (*on_end)(struct Effect *self);

  // called by effect_free
  void (*on_free)(struct Effect *self);

  // renders the effect
  void (*on_render)(struct Effect *self, Renderer *renderer);
} Effect;

Effect effect_init(EffectType type, float duration);
void effect_free(Effect *self);

void effect_apply(Effect *self, AffectableComponent *affectable);
int effect_update(Effect *self, double dt);
void effect_end(Effect *self);
void effect_render(Effect *self, Renderer *renderer);

// function to advance an effect, without ending/freeing like update does
void effect_advance(Effect *self, double dt);

#endif
