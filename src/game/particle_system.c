#include <stdlib.h>
#include "util/mat4.h"
#include "particle_system.h"

static float rand_range(float min, float max) {
	float val = ((float)rand()) / (float)RAND_MAX;
	return val * (max - min) + min;
}

static Vec3 rand_unit(float scale) {
	Vec3 output;
	output.x = 2 * (((float)rand()) / (float)RAND_MAX) - 1;
	output.y = (((float)rand()) / (float)RAND_MAX);
	output.z = 2 * (((float)rand()) / (float)RAND_MAX) - 1;
	vec3_normalize(&output);
	return vec3_scale(output, scale);
}

ParticleSystem *particle_system_new(GameWorld *world, MoveableObject *follow_target,const char *asset_name, int num_particles, float duration, float particle_duration){
    ParticleSystem *self = malloc(sizeof(*self));
    self->world = world;

    self->follow_target = follow_target;

    self->duration = duration;
    self->particle_duration = particle_duration;

    self->num_particles = num_particles;
    Vec3 position = self->follow_target->position, velocity;
    int i;
    for(i = 0;i < num_particles;i++){
		self->particles[i].position = vec3_add(position, rand_unit(2.0));
		self->particles[i].velocity = rand_unit(2.0);
		self->particles[i].duration = rand_range(0, particle_duration);
    }

    self->renderable.asset_id = game_world_get_asset_id(world, asset_name);

    return self;
}

void particle_system_free(ParticleSystem *self){
    free(self);
}

void particle_system_update(ParticleSystem *self, double dt){
    int i;
    Vec3 vel;
    for(i = 0;i < self->num_particles;i++){
        vel = vec3_scale(self->particles[i].velocity, dt);
        self->particles[i].position = vec3_add(self->particles[i].position, vel);
        self->particles[i].duration -= dt;
        if(self->particles[i].duration <= 0.0){
            self->particles[i].position = vec3_add(self->follow_target->position, rand_unit(2.0));
			self->particles[i].velocity = rand_unit(2.0);
            self->particles[i].duration = rand_range(0, self->particle_duration);
        }
    }
}

void particle_system_render(ParticleSystem *self){
    Mat4 model_matrix;
    int i;
    for(i = 0;i < self->num_particles;i++){
        mat4_ident(&model_matrix);
        mat4_translate(&model_matrix, self->particles[i].position);
		mat4_scale(&model_matrix, self->particles[i].duration / self->particle_duration);
        self->renderable.model_matrix = model_matrix;
        renderable_object_render(self->renderable, self->world);
    }
}
