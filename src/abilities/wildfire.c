#include "wildfire.h"
#include "components/affectable_component.h"
#include "effects/effect.h"
#include "effects/burn.h"
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

static Spell *wildfire_new(GameWorld *world, GameObject *user, GameObject *target);
static Spell *wildfire_spread_new(GameWorld *world, GameObject *user, GameObject *target);
static void wildfire_update(Spell *self, double dt);
static void wildfire_on_collide(GameObject *self, GameObject *other);
static void wildfire_spread_on_collide(GameObject *self, GameObject *other);
static void wildfire_apply(GameWorld *world, GameObject *user, Enemy *enemy);
static void wildfire_spread(GameWorld *world, GameObject *origin, Enemy *enemy);

static Spell *wildfire_new(GameWorld *world, GameObject *user, GameObject *target){
    Spell *self = spell_new(world, wildfire_update, spell_render, spell_free);

    if (user->type == GAME_OBJECT_TYPE_PLAYER) {
        Player *player = user;
        self->base_object.position = player->base_object.position;
        self->base_object.direction = player->base_object.direction;
        //start from center of player
        self->speed = 50.0;
        self->base_object.position.y += player->collidable.bounding_box.radius.y;
    }

    self->renderable = renderable_component_init(&self->base_object, "assets/burn_particle", world->renderer);
    self->collidable = collidable_component_init(&self->base_object, game_world_get_model_obb(world, self->renderable.model_id), wildfire_on_collide);
    self->collidable.is_colliding = spell_is_colliding_with_target;

    self->target = target;

    return self;
}

//this is used once the regular wildfire hits an enemy, it then spreads it to other enemies
//around the initial target
static Spell *wildfire_spread_new(GameWorld *world, GameObject *user, GameObject *target){
    Spell *self = spell_new(world, wildfire_update, spell_render, spell_free);

    if (user->type == GAME_OBJECT_TYPE_PLAYER) {
        Player *player = user;
        self->base_object.position = player->base_object.position;
        self->base_object.direction = player->base_object.direction;
        self->speed = 50.0;
        //start from center of player
        self->base_object.position.y += player->collidable.bounding_box.radius.y;
    }else if(user->type == GAME_OBJECT_TYPE_ENEMY) {
        Enemy *enemy = user;
        self->base_object.position = enemy->base_object.position;
        self->base_object.direction = enemy->base_object.direction;
        self->speed = 50.0;
        //start from center of enemy
        self->base_object.position.y += enemy->collidable.bounding_box.radius.y;
    }

    self->renderable = renderable_component_init(&self->base_object, "assets/burn_particle", world->renderer);
    self->collidable = collidable_component_init(&self->base_object, game_world_get_model_obb(world, self->renderable.model_id), wildfire_spread_on_collide);
    self->collidable.is_colliding = spell_is_colliding_with_target;

    self->target = target;

    return self;
}

static void wildfire_update(Spell *self, double dt){
    if (self->target->destroy){
        self->base_object.destroy = 1;
        return;
    }
	spell_update(self, dt);

    Vec3 pos = self->base_object.position;
    Vec3 target_pos = ((Enemy *)(self->target))->base_object.position;
    self->base_object.direction = vec3_sub(target_pos, pos);
    vec3_normalize(&self->base_object.direction);
}

static void wildfire_on_collide(GameObject *self, GameObject *other){
    if (other->type == GAME_OBJECT_TYPE_ENEMY) {
        GameWorld *world = self->world;
        Enemy *enemy = other;

        if (enemy->affectable.effects[EFFECT_TYPE_BURN] != NULL){
            //was fired at enemies who are already affected by burn
            burn_increase_degree(enemy->affectable.effects[EFFECT_TYPE_BURN]);
        }else{
            affectable_component_affect(&enemy->affectable, burn_new(self->world, &enemy->base_object, 1, 4));
        }

        game_world_apply_to_enemies(world, enemy, 10, wildfire_spread);
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
        if(enemy->affectable.effects[EFFECT_TYPE_BURN] == NULL){
            affectable_component_affect(&enemy->affectable, burn_new(self->world, &enemy->base_object, 1, 4));
        }
    }
    self->destroy = 1;
}

static void wildfire_spread(GameWorld *world, GameObject *origin, Enemy *enemy){
    if(enemy->affectable.effects[EFFECT_TYPE_BURN] == NULL){
        //if it doesn't have a burn on it, then shoot a wildfire spread at it 
        game_world_add_object(world, wildfire_spread_new(world, origin, enemy));
    }
}

static void wildfire_apply(GameWorld *world, GameObject *user, Enemy *enemy){
    if(enemy->affectable.effects[EFFECT_TYPE_BURN] != NULL){
        //if it already has a burn on it shoot a wildfire at it
        game_world_add_object(world, wildfire_new(world, user, enemy));
    }
}

void wildfire_use(GameWorld *world, GameObject *user){
    float radius = 10;
    game_world_apply_to_enemies(world, user, radius, wildfire_apply);
}

