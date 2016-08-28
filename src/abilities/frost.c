#include <stdlib.h>
#include "frost.h"
#include "util/random.h"
#include "util/renderer.h"
#include "enemies/enemy.h"
#include "components/affectable_component.h"

//forward declarations
static void frost_on_apply(Effect *self, AffectableComponent *affectable);
static void frost_on_update(Effect *self, AffectableComponent *affectable, double dt);
static void frost_on_render(Effect *self, Renderer *renderer);
static void frost_on_end(Effect *self, AffectableComponent *affectable);
static void frost_on_free(Effect *self);
static int frost_is_over(Effect *self);
//end forward declarations


static void frost_particle_init(Particle *p, Vec3 position, float duration){
    p->position = vec3_add(position, random_length_vec3(1.0));
    p->velocity = (Vec3) { .data = { 0, -1, 0 } };
    p->duration = random_in_rangef(0, duration);
}

Effect *frost_new(GameWorld *world, GameObject *target, float slow_amt_per_degree, float duration){
    Effect *self = effect_new(EFFECT_TYPE_FROST, duration);
    
    self->data = malloc(sizeof(FrostData));
    FrostData *data = self->data;
    data->degree = 1;
    data->slow_amt_per_degree = slow_amt_per_degree;
    data->particle_system = particle_system_new(world, target->position, "assets/frost_particle", 16, duration, duration * 0.4);
    particle_system_set_particle_init(data->particle_system, frost_particle_init);
    particle_system_set_follow_target(data->particle_system, target);
    //this gives ownership to game_world... we don't have to worry about freeing
    game_world_add_particle_system(world, data->particle_system);

    self->on_apply = frost_on_apply;
    self->on_update = frost_on_update;
    self->on_render = frost_on_render;
    self->on_end = frost_on_end;
    self->on_free = frost_on_free;
    self->is_over = frost_is_over;

    return self;
}

void frost_increase_degree(Effect *self){
    self->duration = self->max_duration;

    FrostData *data;
    data = self->data;
    if(data->degree < 3){
        particle_system_double_particles(data->particle_system);
        data->degree = data->degree + 1;
    }
    data->particle_system->duration = self->duration;
}

static void fizzle_particle_init(Particle *p, Vec3 position, float duration){
    p->position = position;
    p->velocity = random_length_vec3(7.0);
    p->duration = random_in_rangef(0, duration);
}

static void frost_on_apply(Effect *self, AffectableComponent *affectable){
	Effect *frost = affectable->effects[EFFECT_TYPE_FROST];
    if (frost != NULL){        
        FrostData *data = frost->data;
        if(data->degree == 3){
            frost->on_end(frost, affectable);
            //TODO turn into permafrost!
            
            // show burst of particles indicating permafrost has hit
            Enemy *enemy = affectable->base_component.container;
            ParticleSystem *ps = particle_system_new(enemy->base_object.world, enemy->collidable.bounding_box.center, "assets/frost_particle", 64, 0.0, 0.75);
            particle_system_set_particle_init(ps, fizzle_particle_init);
            //this gives ownership to game_world... we don't have to worry about freeing
            game_world_add_particle_system(enemy->base_object.world, ps);
        }else{
            frost_increase_degree(frost);
        }
        effect_free(self);
        return;
	} else {
		affectable->effects[EFFECT_TYPE_FROST] = self;
	}

// TODO figure out how to actually do this slow thing...
    //FrostData *data = self->data;
    //data->amt_slowed = affectable_component_slow(affectable, affectable->data->degree * data->slow_amt_per_degree);
}

static void frost_on_update(Effect *self, AffectableComponent *affectable, double dt){
    FrostData *data = self->data;
    self->duration -= dt;
}

static void frost_on_render(Effect *self, Renderer *renderer){

}

static void frost_on_end(Effect *self, AffectableComponent *affectable){
    //assert i != -1
    //TODO undo slow
	effect_free(affectable->effects[EFFECT_TYPE_FROST]);
	affectable->effects[EFFECT_TYPE_FROST] = NULL;
}

static void frost_on_free(Effect *self){
    FrostData *data = self->data;
    //game world will free particle system when it actually ends
    data->particle_system->duration = 0.0;
    data->particle_system = NULL;
    //particle_system_free(data->particle_system);
    free(self->data);
    free(self);
}

static int frost_is_over(Effect *self){
    return self->duration <= 0.0;
}