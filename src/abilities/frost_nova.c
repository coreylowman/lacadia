#include <stdio.h>
#include <stdlib.h>
#include "players/player.h"
#include "enemies/enemy.h"
#include "util/set.h"
#include "game/particle_system.h"
#include "effects/frost.h"
#include "spell.h"
#include "util/random.h"

static void frost_nova_apply(GameWorld *world, GameObject *user, GameObject *target) {
    Enemy *enemy = target;
    // affectable_component_damage(&enemy->affectable, 1);
    affectable_component_affect(&enemy->affectable, frost_new(world, &enemy->base_object, 0.1, 4));
    affectable_component_affect(&enemy->affectable, frost_new(world, &enemy->base_object, 0.1, 4));
}

static void particle_init(Particle *p, Vec3 position, float duration){
    Vec3 direction = random_flat_vec3(1.0);
    p->position = position;
    p->velocity = vec3_scale(direction, 10.0f / duration);
    p->duration = duration;
}

void frost_nova_use(GameWorld *world, GameObject *user){
    float radius = 10.0f;

    game_world_apply(world, GAME_OBJECT_TYPE_ENEMY, user, radius, frost_nova_apply);

    ParticleSystem *ps = particle_system_new(world, user->position, "assets/frost_particle", 64, 0.0, 0.5);
    particle_system_set_particle_init(ps, particle_init);
    game_world_add_object(world, ps);
}

Ability frost_nova_ability = {
	.cooldown = 0,
	.max_cooldown = 2.0,
	.on_use = frost_nova_use
};
