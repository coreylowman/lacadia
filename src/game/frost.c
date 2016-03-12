#include <stdlib.h>
#include "frost.h"
#include "util/random.h"
#include "util/renderer.h"

//forward declarations
static void frost_on_apply(Effect *self, AffectableObject *affectable);
static void frost_on_update(Effect *self, AffectableObject *affectable, double dt);
static void frost_on_render(Effect *self, Renderer *renderer);
static void frost_on_end(Effect *self, AffectableObject *affectable);
static void frost_on_free(Effect *self);
static int frost_is_over(Effect *self);
//end forward declarations


static void frost_particle_init(Particle *p, Vec3 position, float duration){
    p->position = vec3_add(position, random_length_vec3(1.0));
    p->velocity = (Vec3) { .data = { 0, -1, 0 } };
    p->duration = random_in_rangef(0, duration);
}

Effect *frost_new(GameWorld *world, MoveableObject *target, float slow_amt, float duration){
    Effect *self = effect_new(EFFECT_TYPE_FROST, duration);
    
    self->data = malloc(sizeof(FrostData));
    FrostData *data = self->data;
    data->degree = 1;
    data->slow_amt = slow_amt;
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

static void frost_on_apply(Effect *self, AffectableObject *affectable){
    int i = affectable_object_index_of_effect(affectable, EFFECT_TYPE_FROST);
    
    if (i != -1){
        Effect *e = affectable->effects->data[i];
        FrostData *data = e->data;
        if(data->degree == 3){
            //TODO turn into permafrost!
        }else{
            frost_increase_degree(e);
        }
        effect_free(self);
        return;
    }

    set_add(affectable->effects, self);
}

static void frost_on_update(Effect *self, AffectableObject *affectable, double dt){
    FrostData *data = self->data;
    self->duration -= dt;
}

static void frost_on_render(Effect *self, Renderer *renderer){

}

static void frost_on_end(Effect *self, AffectableObject *affectable){
    int i = affectable_object_index_of_effect(affectable, EFFECT_TYPE_FROST);

    //assert i != -1
    //TODO undo slow
    set_remove_at(affectable->effects, i);    
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