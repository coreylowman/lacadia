#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "util/vec3.h"
#include "renderable_object.h"
#include "moveable_object.h"
#include "game_world.h"

#define MAX_PARTICLES 64

typedef struct {
    Vec3 position;
    Vec3 velocity;
    float duration;
} Particle;

typedef struct {
    GameWorld *world;

    MoveableObject *follow_target;

    float duration;
    float particle_duration;

    int num_particles;
    Particle particles[MAX_PARTICLES];

    //the renderable for each particle
    RenderableObject renderable;
} ParticleSystem;

ParticleSystem *particle_system_new(GameWorld *world, MoveableObject *follow_target, const char *asset_name, int num_particles, float duration, float particle_duration);
void particle_system_free(ParticleSystem *self);

void particle_system_update(ParticleSystem *self, double dt);
void particle_system_render(ParticleSystem *self);

#endif
