#include <stdlib.h>
#include "burn.h"
#include "util/random.h"

//forward declarations
static void burn_on_apply(Effect *self, AffectableObject *affectable);
static void burn_on_update(Effect *self, AffectableObject *affectable, double dt);
static void burn_on_render(Effect *self, GameWorld *world);
static void burn_on_end(Effect *self, AffectableObject *affectable);
static void burn_on_free(Effect *self);
static int burn_is_over(Effect *self);
//end forward declarations


static void burn_particle_init(Particle *p, Vec3 position, float duration){
    p->position = vec3_add(position, random_length_vec3(2.0));
    p->velocity = (Vec3) { .data = { 0, 1, 0 } };
    p->duration = random_in_range(0, duration);
}

Effect *burn_new(GameWorld *world, MoveableObject *target, float dmg, float duration){
    Effect *self = effect_new(EFFECT_TYPE_BURN, duration);
    
    self->data = malloc(sizeof(BurnData));
    BurnData *data = self->data;
    data->degree = 1;
    data->dps = dmg;
    data->particle_system = particle_system_new(world, target, "assets/burn_particle", 32, duration, duration * 0.2);
    particle_system_set_particle_init(data->particle_system, burn_particle_init);

    self->on_apply = burn_on_apply;
    self->on_update = burn_on_update;
    self->on_render = burn_on_render;
    self->on_end = burn_on_end;
    self->on_free = burn_on_free;
    self->is_over = burn_is_over;

    return self;
}

static void burn_on_apply(Effect *self, AffectableObject *affectable){
    int i;
    Effect *e;
    for (i = 0; i < affectable->effects->length; i++) {
        if (affectable->effects->data[i] == NULL) continue;
        e = affectable->effects->data[i];
        if (e->type == EFFECT_TYPE_BURN) {
            e->duration = e->max_duration;
            BurnData *data = e->data;
            // data->degree = min(data->degree + 1, 3);
            effect_free(self);
            return;
        }
    }
    set_add(affectable->effects, self);
}

static void burn_on_update(Effect *self, AffectableObject *affectable, double dt){
    BurnData *data = self->data;
    affectable_object_damage(affectable, dt * data->degree * data->dps);
    self->duration -= dt;
    particle_system_update(data->particle_system, dt);
}

static void burn_on_render(Effect *self, GameWorld *world){
    BurnData *data = self->data;
    particle_system_render(data->particle_system);
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
    particle_system_free(data->particle_system);
    free(self->data);
    free(self);
}

static int burn_is_over(Effect *self){
    return self->duration <= 0.0;
}