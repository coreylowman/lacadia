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
    p->duration = random_in_rangef(0, duration);
}

ParticleSystem *particle_system_new(GameWorld *world, Vec3 position,const char *asset_name, int num_particles, float duration, float particle_duration){
    ParticleSystem *self = malloc(sizeof(*self));
    self->base_object = game_object_init(world, GAME_OBJECT_TYPE_PARTICLE_SYSTEM, particle_system_update, particle_system_render, particle_system_free);
    self->base_object.position = position;

    self->follow_target = NULL;

    self->duration = duration;
    self->particle_duration = particle_duration;

    self->num_particles = num_particles;

    self->particle_init = particle_init;
    int i;
    for(i = 0;i < num_particles;i++) self->particle_init(&self->particles[i], position, self->particle_duration);

	self->renderable = renderable_component_init(&self->base_object, asset_name, world->renderer);

    self->scale_over_duration = 1;

    return self;
}

void particle_system_free(GameObject *obj){
    ParticleSystem *self = (ParticleSystem *) obj;
    free(self);
}

void particle_system_double_particles(ParticleSystem *self){
    int i = self->num_particles;
    self->num_particles *= 2;
    for(;i < self->num_particles;i++){
        self->particle_init(
            &self->particles[i], 
            self->follow_target ? self->follow_target->position : self->base_object.position,
            self->particle_duration);
    }
}

void particle_system_set_scale_over_duration(ParticleSystem *self, int scale_over_duration) {
    self->scale_over_duration = scale_over_duration;
}

void particle_system_set_particle_init(ParticleSystem *self, void (*particle_init_arg)(Particle *p, Vec3 position, float duration)){
    self->particle_init = particle_init_arg;
    int i;
    for(i = 0;i < self->num_particles;i++)
        self->particle_init(
            &self->particles[i],
			self->follow_target ? self->follow_target->position : self->base_object.position,
            self->particle_duration);
}

void particle_system_update(GameObject *obj, double dt){
    ParticleSystem *self = (ParticleSystem *)obj;
    int i;
    Vec3 vel;
    self->duration -= dt;
    for(i = 0;i < self->num_particles;i++){
        vel = vec3_scale(self->particles[i].velocity, dt);
        self->particles[i].position = vec3_add(self->particles[i].position, vel);
        self->particles[i].duration -= dt;
        if(self->duration > 0.0 && self->particles[i].duration <= 0.0)
            self->particle_init(
                &self->particles[i],
				self->follow_target ? self->follow_target->position : self->base_object.position,
                self->particle_duration);
    }

    if(particle_system_is_over(self)) {
        self->base_object.destroy = 1;
    }
}

void particle_system_render(GameObject *obj, Renderer *renderer){
    ParticleSystem *self = (ParticleSystem *)obj;
    Mat4 model_matrix;
    int i;
    for(i = 0;i < self->num_particles;i++){
        if(self->particles[i].duration <= 0.0) continue;
        mat4_ident(&model_matrix);
        mat4_translate(&model_matrix, self->particles[i].position);
        if(self->scale_over_duration) {
            mat4_scale(&model_matrix, self->particles[i].duration / self->particle_duration);
        }
        renderable_component_set_model_matrix(&self->renderable, model_matrix);
        component_render((Component *)&self->renderable, renderer);
    }
}

void particle_system_set_follow_target(ParticleSystem *self, GameObject *follow_target){
    self->follow_target = follow_target;
}

int particle_system_is_over(ParticleSystem *self){
    int i, particles_over = 0;
    if(self->duration > 0.0) return 0;

    for(i = 0;i < self->num_particles;i++){
        if(self->particles[i].duration > 0)
            return 0;
    }
    return 1;
}
