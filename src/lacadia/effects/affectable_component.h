#ifndef AFFECTABLE_COMPONENT_H
#define AFFECTABLE_COMPONENT_H

#include "engine/component.h"
#include "effect.h"

typedef struct AffectableComponent {
  Component base_component;

  float health, max_health;
  float speed, max_speed;
  float regen, max_regen;
  float power, max_power;
  float lifesteal, max_lifesteal;

  Effect *effects[EFFECT_TYPE_MAX];
} AffectableComponent;

// initializes the stats in affectable object
AffectableComponent affectable_component_init(GameObject *container,
                                              float max_health, float max_speed,
                                              float max_regen, float max_power,
                                              float max_lifesteal);

// calls the effects on_apply method
void affectable_component_affect(struct AffectableComponent *self,
                                 struct Effect *effect);
void affectable_component_remove(struct AffectableComponent *self,
                                 EffectType type);

void affectable_component_scale_max(struct AffectableComponent *self,
                                    float scale);

float affectable_component_damage(struct AffectableComponent *self, float amt);
float affectable_component_heal(struct AffectableComponent *self, float amt);
float affectable_component_slow(struct AffectableComponent *self, float amt);
float affectable_component_quicken(struct AffectableComponent *self, float amt);

#endif
