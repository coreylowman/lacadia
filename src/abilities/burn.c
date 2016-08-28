#include <stdlib.h>
#include "burn.h"
#include "util/random.h"
#include "util/renderer.h"
#include "components/affectable_component.h"

//forward declarations
static void burn_on_apply(Effect *self, AffectableComponent *affectable);
static void burn_on_update(Effect *self, AffectableComponent *affectable, double dt);
static void burn_on_render(Effect *self, Renderer *renderer);
static void burn_on_end(Effect *self, AffectableComponent *affectable);
static void burn_on_free(Effect *self);
static int burn_is_over(Effect *self);
//end forward declarations


static void burn_particle_init(Particle *p, Vec3 position, float duration){
    p->position = vec3_add(position, random_length_vec3(1.0));
    p->velocity = (Vec3) { .data = { 0, 1, 0 } };
    p->duration = random_in_rangef(0, duration);
}

Effect *burn_new(GameWorld *world, GameObject *target, float dmg, float duration){
    Effect *self = effect_new(EFFECT_TYPE_BURN, duration);
    
    self->data = malloc(sizeof(BurnData));
    BurnData *data = self->data;
    data->degree = 1;
    data->dps = dmg;
    data->particle_system = particle_system_new(world, target->position, "assets/burn_particle", 16, duration, duration * 0.4);
    particle_system_set_particle_init(data->particle_system, burn_particle_init);
    particle_system_set_follow_target(data->particle_system, target);
    //this gives ownership to game_world... we don't have to worry about freeing
    game_world_add_particle_system(world, data->particle_system);

    self->on_apply = burn_on_apply;
    self->on_update = burn_on_update;
    self->on_render = burn_on_render;
    self->on_end = burn_on_end;
    self->on_free = burn_on_free;
    self->is_over = burn_is_over;

    return self;
}

void burn_increase_degree(Effect *self){
    self->duration = self->max_duration;

    BurnData *data;
    data = self->data;
    if(data->degree < 3){
        particle_system_double_particles(data->particle_system);
        data->degree = data->degree + 1;
    }
    data->particle_system->duration = self->duration;
}

static void burn_on_apply(Effect *self, AffectableComponent *affectable){
	Effect *e = affectable->effects[EFFECT_TYPE_BURN];
	
	if (e != NULL){
		BurnData *data = e->data;
		data->particle_system->duration = e->max_duration;
		e->duration = e->max_duration;
		effect_free(self);
		return;
	}
	else {
		affectable->effects[EFFECT_TYPE_BURN] = self;
	}
}

static void burn_on_update(Effect *self, AffectableComponent *affectable, double dt){
    BurnData *data = self->data;
    affectable_component_damage(affectable, dt * data->degree * data->dps);
    self->duration -= dt;
}

static void burn_on_render(Effect *self, Renderer *renderer){

}

static void burn_on_end(Effect *self, AffectableComponent *affectable){
    int i;
    for(i = 0;i < EFFECT_TYPE_MAX;i++){
        if(affectable->effects[i] == NULL) continue;
        if(affectable->effects[i] == self){
			effect_free(affectable->effects[i]);
			affectable->effects[i] = NULL;
            return;
        }
    }
}

static void burn_on_free(Effect *self){
    BurnData *data = self->data;
    //game world will free particle system when it actually ends
	data->particle_system->duration = 0.0;
    data->particle_system = NULL;
    //particle_system_free(data->particle_system);
    free(self->data);
    free(self);
}

static int burn_is_over(Effect *self){
    return self->duration <= 0.0;
}