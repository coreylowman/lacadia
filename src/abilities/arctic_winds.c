#include <stdio.h>
#include <stdlib.h>
#include "players/player.h"
#include "arctic_winds.h"
#include "enemies/enemy.h"
#include "util/set.h"
#include "game/particle_system.h"
#include "effects/frost.h"
#include "spell.h"
#include "util/random.h"

Ability arctic_winds_ability = {
    .cooldown = 0,
    .max_cooldown = 2.0,
    .on_use = arctic_winds_use
};

static void arctic_winds_apply(GameWorld *world, GameObject *user, GameObject *target) {
    Enemy *enemy = target;
    Vec3 to_enemy = vec3_sub(enemy->base_object.position, user->position);
    float degrees_between = vec3_degrees_between(user->direction, to_enemy);
    if (degrees_between <= 45) {
        affectable_component_damage(&enemy->affectable, 2);
        affectable_component_affect(&enemy->affectable, frost_new(world, &enemy->base_object, 0.1, 4));
    }
}

// todo be able to add custom args to particle_init
static Vec3 direction;

static void particle_init(Particle *p, Vec3 position, float duration){
	float t = random_in_rangef(1.0f, 3.0f);
    p->position = position;
	Vec3 pdirection = vec3_scale(direction, t * 10.0f / duration);
    p->velocity = vec3_rotate_around_y(pdirection, random_in_rangef(-22.5, 22.5));
    p->duration = duration / t;
}

void arctic_winds_use(GameWorld *world, GameObject *user){
    float radius = 10.0f;

    direction = user->direction;
    direction.y = 0;
    vec3_normalize(&direction);
    game_world_apply(world, GAME_OBJECT_TYPE_ENEMY, user, radius, arctic_winds_apply);

    ParticleSystem *ps = particle_system_new(world, user->position, "assets/frost_particle", 64, 0.0, 0.75);
    particle_system_set_particle_init(ps, particle_init);
    particle_system_set_scale_over_duration(ps, 0);
    game_world_add_object(world, ps);
}
