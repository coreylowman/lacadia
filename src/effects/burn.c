#include <stdlib.h>
#include "burn.h"
#include "util/random.h"
#include "util/renderer.h"
#include "components/affectable_component.h"

static void burn_on_apply(Effect *self, AffectableComponent *affectable);
static void burn_on_update(Effect *self, double dt);
static void burn_on_render(Effect *self, Renderer *renderer);
static void burn_on_free(Effect *self);

static void burn_particle_init(Particle *p, Vec3 position, float duration){
    p->position = vec3_add(position, random_length_vec3(1.0));
    p->velocity = (Vec3) { .data = { 0, 1, 0 } };
    p->duration = random_in_rangef(0, duration);
}

Burn *burn_new(GameWorld *world, GameObject *target, float dmg, float duration){
    Burn *self = malloc(sizeof(*self));

    self->base_effect = effect_init(EFFECT_TYPE_BURN, duration);
    
    self->degree = 1;
    self->dps = dmg;

    self->particle_system = particle_system_new(world, target->position, "assets/burn_particle", 16, duration, duration * 0.4);
    particle_system_set_particle_init(self->particle_system, burn_particle_init);
    particle_system_set_follow_target(self->particle_system, target);

    self->base_effect.on_apply = burn_on_apply;
    self->base_effect.on_update = burn_on_update;
    self->base_effect.on_render = burn_on_render;
    self->base_effect.on_end = NULL;
    self->base_effect.on_free = burn_on_free;

    return self;
}

void burn_increase_degree(Effect *effect){
    Burn *self = effect;

    effect->duration = effect->max_duration;
    self->particle_system->duration = effect->duration;

	if (self->degree < 3){
		particle_system_double_particles(self->particle_system);
		self->degree = self->degree + 1;
	}
}

static void burn_on_apply(Effect *self, AffectableComponent *affectable){
    Burn *burn = affectable->effects[EFFECT_TYPE_BURN];
    
    if (burn != NULL){
        // refresh the duration of the existing burn
        burn->particle_system->duration = burn->base_effect.max_duration;
        burn->base_effect.duration = burn->base_effect.max_duration;

        // free the effect we were trying to add, since we just modified the burn
        // already on the target
        effect_free(self);
        return;
    } else {
        affectable->effects[EFFECT_TYPE_BURN] = self;
    }
}

static void burn_on_update(Effect *effect, double dt){
    Burn *self = effect;
    AffectableComponent *affectable = self->base_effect.container;

    float amt = affectable_component_damage(affectable, dt * self->degree * self->dps);
    effect->amount_affected += amt;

    particle_system_update(self->particle_system, dt);
}

static void burn_on_render(Effect *effect, Renderer *renderer){
    Burn *self = effect;
    particle_system_render(self->particle_system, renderer);
}

static void burn_on_free(Effect *effect){
    Burn *self = effect;
    particle_system_free(self->particle_system);
    free(self);
}
