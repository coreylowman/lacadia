#include <stdlib.h>
#include "util/mat4.h"
#include "particle_system.h"
#include "util/random.h"

static Vec3 rand_scale_vec3(float scale){
    return vec3_scale(random_unit_vec3(), scale);
}

static void particle_init(Particle *p, Vec3 position, float duration){
    p->position = vec3_add(position, random_unit_vec3());
    p->velocity = random_unit_vec3();
    p->duration = random_in_range(0, duration);
}

ParticleSystem *particle_system_new(GameWorld *world, MoveableObject *follow_target,const char *asset_name, int num_particles, float duration, float particle_duration){
    ParticleSystem *self = malloc(sizeof(*self));
    self->world = world;

    self->follow_target = follow_target;

    self->duration = duration;
    self->particle_duration = particle_duration;

    self->num_particles = num_particles;

    self->particle_init = particle_init;
    int i;
    for(i = 0;i < num_particles;i++) self->particle_init(&self->particles[i], self->follow_target->position, self->particle_duration);

    self->renderable.asset_id = game_world_get_asset_id(world, asset_name);

    return self;
}

void particle_system_free(ParticleSystem *self){
    free(self);
}

void particle_system_double_particles(ParticleSystem *self){
    int i = self->num_particles;
    self->num_particles *= 2;
    for(;i < self->num_particles;i++){
        self->particle_init(&self->particles[i], self->follow_target->position, self->particle_duration);
    }
}

void particle_system_set_particle_init(ParticleSystem *self, void (*particle_init_arg)(Particle *p, Vec3 position, float duration)){
    self->particle_init = particle_init_arg;
    int i;
    for(i = 0;i < self->num_particles;i++) self->particle_init(&self->particles[i], self->follow_target->position, self->particle_duration);
}

void particle_system_update(ParticleSystem *self, double dt){
    int i;
    Vec3 vel;
    for(i = 0;i < self->num_particles;i++){
        vel = vec3_scale(self->particles[i].velocity, dt);
        self->particles[i].position = vec3_add(self->particles[i].position, vel);
        self->particles[i].duration -= dt;
        if(self->particles[i].duration <= 0.0) self->particle_init(&self->particles[i], self->follow_target->position, self->particle_duration);
    }
}

void particle_system_render(ParticleSystem *self){
    Mat4 model_matrix;
    int i;
    for(i = 0;i < self->num_particles;i++){
        mat4_ident(&model_matrix);
        mat4_translate(&model_matrix, self->particles[i].position);
        mat4_scale(&model_matrix, self->particles[i].duration / self->particle_duration);
        mat4_transpose(&model_matrix);
        self->renderable.model_matrix = model_matrix;
        renderable_object_render(self->renderable, self->world);
    }
}
