#include <stdio.h>
#include <stdlib.h>
#include "players/player.h"
#include "icicle.h"
#include "enemies/enemy.h"
#include "util/set.h"
#include "game/particle_system.h"
#include "spell.h"
#include "util/random.h"
#include "frost.h"

Ability icicle_ability = {
    .cooldown = 0,
    .max_cooldown = 0.33,
    .on_use = icicle_use
};

//forward declarations
static Spell *icicle_new(GameWorld *world, GameObject *user);
static void icicle_update(Spell *self, double dt);
static void icicle_on_collide(GameObject *self, GameObject *other);
//end forward declarations


void icicle_use(GameWorld *world, GameObject *user){
    game_world_add_spell(world, icicle_new(world, user));
}

static Spell *icicle_new(GameWorld *world, GameObject *user){
    Spell *self = spell_new(world);

    if(user->type == GAME_OBJECT_TYPE_PLAYER){
        Player *player = user;
        self->base_object.position = player->base_object.position;
		self->base_object.direction = player->base_object.direction;
        self->speed = 30.0;
		self->base_object.position = vec3_add(player->base_object.position, player->base_object.direction);
		self->base_object.position.y += 0.5 * player->collidable.bounding_box.radius.y;
        self->caster_type = GAME_OBJECT_TYPE_PLAYER;
    }else{
        self->caster_type = GAME_OBJECT_TYPE_ENEMY;
    }

	self->renderable = renderable_component_init(&self->base_object, "assets/icicle", world->renderer);
	self->collidable = collidable_component_init(&self->base_object, game_world_get_model_obb(world, self->renderable.model_id), icicle_on_collide);
    self->collidable.is_colliding = spell_is_colliding;

    self->target = NULL;

    self->on_update = icicle_update;

    return self;
}

static void icicle_update(Spell *self, double dt){
    //just have to move
}

static void fizzle_particle_init(Particle *p, Vec3 position, float duration){
    p->position = position;
    p->velocity = random_length_vec3(3.0);
    p->duration = random_in_rangef(0, duration);
}

static void icicle_on_collide(GameObject *self, GameObject *other){
    if(other->type == GAME_OBJECT_TYPE_ENEMY){
        Enemy *enemy = other;
        affectable_component_damage(&enemy->affectable, 1);
        affectable_component_affect(&enemy->affectable, frost_new(self->world, &enemy->base_object, 0.1, 4));
        self->destroy = 1;
    }else if(other->type == GAME_OBJECT_TYPE_WALL){
        Spell *icicle = self;
        ParticleSystem *ps = particle_system_new(self->world, icicle->collidable.bounding_box.center, "assets/frost_particle", 32, 0.0, 0.75);
        particle_system_set_particle_init(ps, fizzle_particle_init);
        //this gives ownership to game_world... we don't have to worry about freeing
        game_world_add_particle_system(self->world, ps);
        self->destroy = 1;
    }
}



