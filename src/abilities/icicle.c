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
    .max_cooldown = 0,
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
        self->moveable = player->moveable;
        self->moveable.speed = 30.0;
        self->moveable.position = vec3_add(player->moveable.position, player->moveable.direction);
        self->moveable.position.y += 0.5 * player->collidable.bounding_box.radius.y;
        self->caster_type = GAME_OBJECT_TYPE_PLAYER;
    }else{
        self->caster_type = GAME_OBJECT_TYPE_ENEMY;
    }

    self->renderable.model_id = game_world_get_model_id(world, "assets/icicle");
    renderable_object_update(&self->renderable, self->moveable);

    self->collidable.container = self;
    self->collidable.on_collide = icicle_on_collide;
    self->collidable.is_colliding = spell_is_colliding;
    self->collidable.bounding_box = game_world_get_model_obb(world, self->renderable.model_id);
    collidable_object_update(&self->collidable, self->moveable);

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
        affectable_object_damage(&enemy->affectable, 1);
        affectable_object_affect(&enemy->affectable, frost_new(self->world, &enemy->moveable, 1, 4));
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



