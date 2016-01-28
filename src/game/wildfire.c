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
static void wildfire_spread(GameWorld *world, Enemy *enemy);
//end forward declarations

static Spell *wildfire_new(GameWorld *world, GameObject *user, GameObject *target){
	Spell *self = spell_new(world);

	if (user->type == GAME_OBJECT_TYPE_PLAYER) {
		Player *player = user->container;
		self->moveable = player->moveable;
		//start from center of player
		self->moveable.position.y += player->collidable.bounding_box.radius.y;
	}

	self->renderable.asset_id = game_world_get_asset_id(world, "assets/burn_particle");
	renderable_object_update(&self->renderable, self->moveable);

	self->collidable.container = self->base_object;
	self->collidable.on_collide = wildfire_on_collide;
	self->collidable.is_colliding = spell_is_colliding_with_target;
	self->collidable.bounding_box = game_world_get_asset_obb(world, self->renderable.asset_id);
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
		Player *player = user->container;
		self->moveable = player->moveable;
		//start from center of player
		self->moveable.position.y += player->collidable.bounding_box.radius.y;
	}

	self->renderable.asset_id = game_world_get_asset_id(world, "assets/burn_particle");
	renderable_object_update(&self->renderable, self->moveable);

	self->collidable.container = self->base_object;
	self->collidable.on_collide = wildfire_spread_on_collide;
	self->collidable.is_colliding = spell_is_colliding_with_target;
	self->collidable.bounding_box = game_world_get_asset_obb(world, self->renderable.asset_id);
	collidable_object_update(&self->collidable, self->moveable);

	self->target = target;

	self->on_update = wildfire_update;

	return self;
}

static void wildfire_update(Spell *self, double dt){
	//TODO what happens if you fire a targetted spell at an enemy and it dies before the spell gets to it?
	//always move towards target
	Vec3 pos = self->moveable.position;
	Vec3 target_pos = ((Enemy *)(self->target->container))->moveable.position;
	self->moveable.direction = vec3_sub(target_pos, pos);
	vec3_normalize(&self->moveable.direction);

	moveable_object_update(&self->moveable, dt);
	renderable_object_update(&self->renderable, self->moveable);
	collidable_object_update(&self->collidable, self->moveable);
}

static void wildfire_on_collide(GameObject *self, GameObject *other){
	if (other->type == GAME_OBJECT_TYPE_ENEMY) {
		Enemy *enemy = other->container;
		int i;
		Effect *e;
		BurnData *data;

		for (i = 0; i < enemy->affectable.effects->length; i++) {
			if (enemy->affectable.effects->data[i] == NULL) continue;
			e = enemy->affectable.effects->data[i];

			if (e->type == EFFECT_TYPE_BURN) {
				data = e->data;
				data->degree = min(3, data->degree);
				e->duration = e->max_duration;
				game_world_apply_to_enemies(self->world, enemy->collidable.bounding_box.center, 5, wildfire_spread);
				return;
			}
		}
	}
	self->destroy = 1;
}

static void wildfire_spread_on_collide(GameObject *self, GameObject *other){
	if (other->type == GAME_OBJECT_TYPE_ENEMY) {
		Enemy *enemy = other->container;
		int i;
		Effect *e;

		//have to check for a burn again, even though we only fired at ones
		//without a burn on them, this spell could fire multiple spreads at the
		//same target. so in the time it took this one to collide, another one
		//could've already applied
		for (i = 0; i < enemy->affectable.effects->length; i++) {
			if (enemy->affectable.effects->data[i] == NULL) continue;
			e = enemy->affectable.effects->data[i];

			//return if it now has a burn on it
			if (e->type == EFFECT_TYPE_BURN) return;
		}

		//if it doesn't have a burn, then apply one!
		affectable_object_affect(&enemy->affectable, burn_new(self->world, &enemy->moveable, 1, 4));
	}
	self->destroy = 1;
}

static void wildfire_spread(GameWorld *world, Enemy *enemy){
	int i;
	Effect *e;
	for (i = 0; i < enemy->affectable.effects->length; i++) {
		if (enemy->affectable.effects->data[i] == NULL) continue;
		e = enemy->affectable.effects->data[i];

		if (e->type == EFFECT_TYPE_BURN) {
			return;
		}
	}

	//if it doesn't have a burn on it, then shoot a wildfire spread at it 
	game_world_add_spell(world, wildfire_spread_new(world, world->player->base_object, enemy->base_object));
}

static void wildfire_apply(GameWorld *world, Enemy *enemy){
	int i;
	Effect *e;
	for (i = 0; i < enemy->affectable.effects->length; i++) {
		if (enemy->affectable.effects->data[i] == NULL) continue;
		e = enemy->affectable.effects->data[i];

		if (e->type == EFFECT_TYPE_BURN) {
			//shoot a wildfire at it
			game_world_add_spell(world, wildfire_new(world, world->player->base_object, enemy->base_object));
			return;
		}
	}
}

void wildfire_use(GameWorld *world, GameObject *user){
	Player *player = user->container;
	float radius = 10;
	game_world_apply_to_enemies(world, player->collidable.bounding_box.center, 10, wildfire_apply);
}

