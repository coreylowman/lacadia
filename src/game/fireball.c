#include <stdio.h>
#include <stdlib.h>
#include "players/player.h"
#include "fireball.h"
#include "enemies/enemy.h"
#include "util/set.h"
#include "particle_system.h"
#include "burn.h"
#include "spell.h"

Ability fireball_ability = {
    .cooldown = 0,
    .max_cooldown = 0,
    .on_use = fireball_use
};

//forward declarations
static Spell *fireball_new(GameWorld *world, GameObject *user);
static void fireball_update(Spell *self, double dt);
static void fireball_on_collide(GameObject *self, GameObject *other);
//end forward declarations


void fireball_use(GameWorld *world, GameObject *user){
    game_world_add_spell(world, fireball_new(world, user));
}

static Spell *fireball_new(GameWorld *world, GameObject *user){
    Spell *self = spell_new(world);

    if(user->type == GAME_OBJECT_TYPE_PLAYER){
        Player *player = user->container;
		self->moveable = player->moveable;
        self->moveable.speed = 10.0;
        self->moveable.position = vec3_add(player->moveable.position, player->moveable.direction);
		self->moveable.position.y += 0.5 * player->collidable.bounding_box.radius.y;
    }

    self->renderable.asset_id = game_world_get_asset_id(world, "assets/fireball");
    renderable_object_update(&self->renderable, self->moveable);

    self->collidable.container = self->base_object;
    self->collidable.on_collide = fireball_on_collide;
    self->collidable.is_colliding = collidable_object_is_colliding;
    self->collidable.bounding_box = game_world_get_asset_obb(world, self->renderable.asset_id);
    collidable_object_update(&self->collidable, self->moveable);

    self->target = NULL;

    self->on_update = fireball_update;

    return self;
}

static void fireball_update(Spell *self, double dt){
    //just have to move
}

static void fireball_on_collide(GameObject *self, GameObject *other){
    if(other->type == GAME_OBJECT_TYPE_ENEMY){
        Enemy *enemy = other->container;
        affectable_object_damage(&enemy->affectable, 1);
	    affectable_object_affect(&enemy->affectable, burn_new(self->world, &enemy->moveable, 1, 4));
    }
    self->destroy = 1;
}



