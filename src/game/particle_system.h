#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "util/vec3.h"
#include "components/renderable_component.h"
#include "game_world.h"
#include "util/renderer.h"

#define MAX_PARTICLES 256

typedef struct {
    Vec3 position;
    Vec3 velocity;
    float duration;
} Particle;

typedef struct {
    GameObject base_object;

    GameObject *follow_target;

    float duration;
    float particle_duration;

    void (*particle_init)(Particle *p, Vec3 position, float duration);
    int num_particles;
    Particle particles[MAX_PARTICLES];

    int scale_over_duration;

    //the renderable for each particle
    RenderableComponent renderable;
} ParticleSystem;

ParticleSystem *particle_system_new(GameWorld *world, Vec3 position, const char *asset_name, int num_particles, float duration, float particle_duration);
void particle_system_free(GameObject *self);

void particle_system_set_follow_target(ParticleSystem *self, GameObject *follow_target);
void particle_system_set_scale_over_duration(ParticleSystem *self, int scale_over_duration);
void particle_system_double_particles(ParticleSystem *self);

void particle_system_set_particle_init(ParticleSystem *self, void (*particle_init)(Particle *p, Vec3 position, float duration));

void particle_system_update(GameObject *self, double dt);
void particle_system_render(GameObject *self, Renderer *renderer);

int particle_system_is_over(ParticleSystem *self);

#endif
