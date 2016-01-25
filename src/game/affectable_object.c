#include <stdlib.h>
#include "affectable_object.h"

void affectable_object_init(AffectableObject *self){
    self->stats.health = self->stats.max_health;
    self->stats.speed = self->stats.max_speed;
    self->stats.regen = self->stats.max_regen;
    self->stats.power = self->stats.max_power;
    self->stats.lifesteal = self->stats.max_lifesteal;
    self->effects = set_new(effect_free);
}

void affectable_object_affect(AffectableObject *self, Effect *effect){
    effect->on_apply(effect, self);
}

void affectable_object_update(AffectableObject *self, double dt){
    int i;
    Effect *e;
    for(i = 0;i < self->effects->length;i++){
        if(self->effects->data[i] == NULL) continue;
        e = self->effects->data[i];
        e->on_update(e, self, dt);
        if(e->is_over(e))
            e->on_end(e, self);
    }
}

void affectable_object_render(AffectableObject self, GameWorld *world){
    int i;
    Effect *e;
    for(i = 0;i < self.effects->length;i++){
        if(self.effects->data[i] == NULL) continue;
        e = self.effects->data[i];
        e->on_render(e, world);
    }
}

void affectable_object_print(AffectableObject self){
    printf("health: %f %f\n", self.stats.health, self.stats.max_health);
    // printf("speed: %f %f\n", self.stats.speed, self.stats.max_speed);
    // printf("regen: %f %f\n", self.stats.regen, self.stats.max_regen);
    // printf("power: %f %f\n", self.stats.power, self.stats.max_power);
    // printf("lifesteal: %f %f\n", self.stats.lifesteal, self.stats.max_lifesteal);
}

