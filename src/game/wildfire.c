#include "wildfire.h"
#include "affectable_object.h"
#include "effect.h"
#include "burn.h"
#include "enemies/enemy.h"
#include "players/player.h"
#include "ability.h"
#include "spell.h"
#include "util/vec3.h"

Ability wildfire_ability = {
	.cooldown = 0,
	.max_cooldown = 0,
	.on_use = wildfire_use
};

//forward declarations
static Spell *wildfire_new(GameWorld *world, GameObject *user, GameObject *target);
static Spell *wildfire_spread_new(GameWorld *world, GameObject *user, GameObject *target);
static void wildfire_update(Spell *self, double dt);
static void wildfire_on_collide(GameObject *self, GameObject *other);
static void wildfire_spread_on_collide(GameObject *self, GameObject *other);
static void wildfire_apply(GameWorld *world, Enemy *enemy);
static void wildfire_spread(GameWorld *world, Enemy *origin, Enemy *enemy);
//end forward declarations

static Spell *wildfire_new(GameWorld *world, GameObject *user, GameObject *target){
	Spell *self = spell_new(world);

	if (user->type == GAME_OBJECT_TYPE_PLAYER) {
		Player *player = user;
		self->moveable = player->moveable;
		//start from center of player
		self->moveable.speed = 50.0;
		self->moveable.position.y += player->collidable.bounding_box.radius.y;
	}

	self->renderable.model_id = game_world_get_model_id(world, "assets/burn_particle");
	renderable_object_update(&self->renderable, self->moveable);

	self->collidable.container = self;
	self->collidable.on_collide = wildfire_on_collide;
	self->collidable.is_colliding = spell_is_colliding_with_target;
	self->collidable.bounding_box = game_world_get_model_obb(world, self->renderable.model_id);
	collidable_object_update(&self->collidable, self->moveable);

	self->target = target;

	self->on_update = wildfire_update;

	return self;
}

//this is used once the regular wildfire hits an enemy, it then spreads it to other enemies
//around the initial target
static Spell *wildfire_spread_new(GameWorld *world, GameObject *user, GameObject *target){
	Spell *self = spell_new(world);

	if (user->type == GAME_OBJECT_TYPE_PLAYER) {
		Player *player = user;
		self->moveable = player->moveable;
		self->moveable.speed = 50.0;
		//start from center of player
		self->moveable.position.y += player->collidable.bounding_box.radius.y;
	}else if(user->type == GAME_OBJECT_TYPE_ENEMY) {
		Enemy *enemy = user;
		self->moveable = enemy->moveable;
		self->moveable.speed = 50.0;
		//start from center of enemy
		self->moveable.position.y += enemy->collidable.bounding_box.radius.y;
	}

	self->renderable.model_id = game_world_get_model_id(world, "assets/burn_particle");
	renderable_object_update(&self->renderable, self->moveable);

	self->collidable.container = self;
	self->collidable.on_collide = wildfire_spread_on_collide;
	self->collidable.is_colliding = spell_is_colliding_with_target;
	self->collidable.bounding_box = game_world_get_model_obb(world, self->renderable.model_id);
	collidable_object_update(&self->collidable, self->moveable);

	self->target = target;

	self->on_update = wildfire_update;

	return self;
}

static void wildfire_update(Spell *self, double dt){
	if (self->target->destroy){
		self->base_object.destroy = 1;
		return;
	}
	Vec3 pos = self->moveable.position;
	Vec3 target_pos = ((Enemy *)(self->target))->moveable.position;
	self->moveable.direction = vec3_sub(target_pos, pos);
	vec3_normalize(&self->moveable.direction);
}

static void wildfire_on_collide(GameObject *self, GameObject *other){
	if (other->type == GAME_OBJECT_TYPE_ENEMY) {
		GameWorld *world = self->world;
		Enemy *enemy = other;
		int i, j;
		Effect *e;
		CollidableObject *collidable;
		GameObject *object;

		i = affectable_object_index_of_effect(&enemy->affectable, EFFECT_TYPE_BURN);
		//was fired at enemies who are already affected by burn
		if (i != -1){
			e = enemy->affectable.effects->data[i];
			burn_increase_degree(e);
		}else{
			affectable_object_affect(&enemy->affectable, burn_new(self->world, &enemy->moveable, 1, 4));
		}

		//search for enemies with 5 radius of this enemy
	    for(j = 0;j < world->collidables->length;j++){
	        if(world->collidables->data[j] == NULL) continue;
	        collidable = world->collidables->data[j];
	        object = collidable->container;
	        if(object->type == GAME_OBJECT_TYPE_ENEMY
	            && vec3_within_dist(collidable->bounding_box.center, enemy->collidable.bounding_box.center, 10))
	        {
	            wildfire_spread(world, (Enemy *)other, (Enemy *)object);
	        }
	    }	    
	}
	self->destroy = 1;
}

static void wildfire_spread_on_collide(GameObject *self, GameObject *other){
	if (other->type == GAME_OBJECT_TYPE_ENEMY) {
		Enemy *enemy = other;
		//have to check for a burn again, even though we only fired at ones
		//without a burn on them, this spell could fire multiple spreads at the
		//same target. so in the time it took this one to collide, another one
		//could've already applied
		//if it doesn't have a burn, then apply one!
		if(affectable_object_index_of_effect(&enemy->affectable, EFFECT_TYPE_BURN) == -1){
			affectable_object_affect(&enemy->affectable, burn_new(self->world, &enemy->moveable, 1, 4));
		}
	}
	self->destroy = 1;
}

static void wildfire_spread(GameWorld *world, Enemy *origin, Enemy *enemy){
	if(affectable_object_index_of_effect(&enemy->affectable, EFFECT_TYPE_BURN) == -1){
		//if it doesn't have a burn on it, then shoot a wildfire spread at it 
		game_world_add_spell(world, wildfire_spread_new(world, origin, enemy));
	}
}

static void wildfire_apply(GameWorld *world, Enemy *enemy){
	if(affectable_object_index_of_effect(&enemy->affectable, EFFECT_TYPE_BURN) != -1){
		//if it already has a burn on it shoot a wildfire at it
		game_world_add_spell(world, wildfire_new(world, world->player, enemy));
	}
}

void wildfire_use(GameWorld *world, GameObject *user){
	Player *player = user;
	float radius = 10;
	game_world_apply_to_enemies(world, player->collidable.bounding_box.center, 10, wildfire_apply);
}

