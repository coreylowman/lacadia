#include <stdlib.h>
#include "burn.h"
#include "util/random.h"
#include "util/renderer.h"

//forward declarations
static void burn_on_apply(Effect *self, AffectableObject *affectable);
static void burn_on_update(Effect *self, AffectableObject *affectable, double dt);
static void burn_on_render(Effect *self, Renderer *renderer);
static void burn_on_end(Effect *self, AffectableObject *affectable);
static void burn_on_free(Effect *self);
static int burn_is_over(Effect *self);
//end forward declarations


static void burn_particle_init(Particle *p, Vec3 position, float duration){
    p->position = vec3_add(position, random_length_vec3(1.0));
    p->velocity = (Vec3) { .data = { 0, 1, 0 } };
    p->duration = random_in_rangef(0, duration);
}

Effect *burn_new(GameWorld *world, MoveableObject *target, float dmg, float duration){
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

static void burn_on_apply(Effect *self, AffectableObject *affectable){
	int i = affectable_object_index_of_effect(affectable, EFFECT_TYPE_BURN);;
    
	if (i != -1){
		Effect *e = affectable->effects->data[i];
		BurnData *data = e->data;
		data->particle_system->duration = e->max_duration;
		e->duration = e->max_duration;
		effect_free(self);
		return;
	}

    set_add(affectable->effects, self);
}

static void burn_on_update(Effect *self, AffectableObject *affectable, double dt){
    BurnData *data = self->data;
    affectable_object_damage(affectable, dt * data->degree * data->dps);
    self->duration -= dt;
}

static void burn_on_render(Effect *self, Renderer *renderer){

}

static void burn_on_end(Effect *self, AffectableObject *affectable){
    int i;
    for(i = 0;i < affectable->effects->length;i++){
        if(affectable->effects->data[i] == NULL) continue;
        if(affectable->effects->data[i] == self){
            set_remove_at(affectable->effects, i);
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