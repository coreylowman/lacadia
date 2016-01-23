#include "players/player.h"
#include "fireball.h"

Ability fireball_ability = {
    .cooldown = 0,
    .max_cooldown = 0,
    .on_use = fireball_use
};

void fireball_use(GameWorld *world, GameObject *user){
    game_world_add_spell(world, fireball_new(world, user));
}

Spell *fireball_new(GameWorld *world, GameObject *user){
    Spell *self = spell_new(world);

    if(user->type == GAME_OBJECT_TYPE_PLAYER){
        Player *player = user->container;
		self->moveable = player->moveable;
        self->moveable.position = vec3_add(player->moveable.position, vec3_scale(player->moveable.direction, 5));
    }

    self->renderable.asset_id = game_world_get_asset_id(world, "assets/fireball");
    renderable_object_update(&self->renderable, self->moveable);

    self->collidable.self = self->base_object;
    self->collidable.on_collide = fireball_on_collide;
    self->collidable.bounding_box = game_world_get_asset_obb(world, self->renderable.asset_id);
    collidable_object_update(&self->collidable, self->moveable);

    self->on_update = fireball_update;

    return self;
}

void fireball_update(Spell *self, double dt){
    moveable_object_update(&self->moveable, dt);
    renderable_object_update(&self->renderable, self->moveable);
    collidable_object_update(&self->collidable, self->moveable);
}

void fireball_on_collide(GameObject *self, GameObject *other){
    self->destroy = 1;
}
