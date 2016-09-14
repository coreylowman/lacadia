#include <stdio.h>
#include <stdlib.h>
#include "players/player.h"
#include "enemies/enemy.h"
#include "util/set.h"
#include "game/particle_system.h"
#include "effects/effect.h"
#include "effects/frost.h"
#include "components/affectable_component.h"
#include "spell.h"
#include "util/random.h"

// todo make this look better... have it go faster or something
static void particle_init(Particle *p, Vec3 position, float duration){
    Vec3 direction = random_flat_vec3(1.5 / duration);
    p->position = vec3_add(position, direction);
    p->velocity = vec3_scale(direction, -1);
    p->duration = duration;
}

static void crystalize_apply(GameWorld *world, GameObject *user, GameObject *target) {
    Enemy *enemy = (Enemy *)target;
    float damage = 2.0f;
    float dmg_per_degree = 1.0f;

    AffectableComponent *affectable = &enemy->affectable;

    if (affectable->effects[EFFECT_TYPE_FROST] != NULL) {
        Frost *frost = (Frost *)affectable->effects[EFFECT_TYPE_FROST];
        damage += frost->degree * dmg_per_degree;
    }

    if (affectable->effects[EFFECT_TYPE_PERMAFROST] != NULL) {
        damage += 5 * dmg_per_degree;
    }

    affectable_component_damage(&enemy->affectable, damage);

    ParticleSystem *ps = particle_system_new(world, enemy->base_object.position, "assets/frost_particle", 64, 0.0, 0.75);
    particle_system_set_particle_init(ps, particle_init);
	particle_system_set_follow_target(ps, &enemy->base_object);
    //particle_system_set_scale_over_duration(ps, 1);
    game_world_add_object(world, (GameObject *)ps);
}

void crystalize_use(GameWorld *world, GameObject *user){
    float radius = 10.0f;

    game_world_apply(world, GAME_OBJECT_TYPE_ENEMY, user, radius, crystalize_apply);
}

Ability crystalize_ability = {
	.cooldown = 0,
	.max_cooldown = 4.0,
	.on_use = crystalize_use
};

