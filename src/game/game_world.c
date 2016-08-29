#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

#include "level.h"
#include "players/player.h"
#include "enemies/enemy.h"
#include "util/array_list.h"
#include "util/obb.h"
#include "util/string_helpers.h"
#include "game_object.h"
#include "abilities/spell.h"
#include "game_world.h"
#include "components/collidable_component.h"
#include "game/particle_system.h"
#include "wall.h"
#include "util/camera.h"
#include "util/inputs.h"

extern int width, height;

Vec3 light_position;

GameWorld *game_world_new(){
    GameWorld *self = malloc(sizeof(*self));
	self->game_objects = set_new(game_object_free);
    self->collidables = set_new(NULL); //these collidables are pointers to other objects collidables... this set doesn't have ownership
    self->indices = set_new(free);

    self->renderer = renderer_new();

    self->inputs = inputs_init();
    camera_init(&self->camera, width, height);
    self->level = level_new(self);

    return self;
}

void game_world_free(GameWorld *self){
	set_free(self->game_objects);
    set_free(self->collidables);
    set_free(self->indices);

	renderer_free(self->renderer);

    level_free(self->level);
    player_free(self->player);

    free(self);
}

static void destroy_collidable(GameWorld *self, int i){
    int index = *(int *)self->indices->data[i];
	CollidableComponent *c = self->collidables->data[i];
	set_remove_at(self->game_objects, index);
    set_remove_at(self->collidables, i);
    set_remove_at(self->indices, i);
}

void game_world_update(GameWorld *self, double dt){
    int i, j;
    CollidableComponent *c1, *c2;

    self->dt = dt;

    // todo make it so player isn't handled specially. this will involve changes in main.c also
    // and other places in this file where its handled specially
    game_object_update(self->player, dt);

    GameObject *obj;
	for (i = 0; i < self->game_objects->length; i++) {
		if (self->game_objects->data[i] == NULL) continue;
		obj = self->game_objects->data[i];
		game_object_update(obj, dt);
	}

	// TODO improve this whole thing with the collidables, indices set and how things are removed
    // this handles collisions as well as removing things from their sets and collidables
    for(i = 0;i < self->collidables->length - 1;i++){
        if(self->collidables->data[i] == NULL) continue;
        c1 = self->collidables->data[i];

        if(c1->base_component.container->destroy){
            destroy_collidable(self, i);
            continue;
        }

        for(j = i + 1;j < self->collidables->length;j++){
            if(self->collidables->data[j] == NULL) continue;
            c2 = self->collidables->data[j];
            
			if (c2->base_component.container->destroy){
                destroy_collidable(self, j);
                continue;
            }

            if(c1->is_colliding(*c1, *c2) && c2->is_colliding(*c2, *c1)){
				c1->on_collide(c1->base_component.container, c2->base_component.container);
				c2->on_collide(c2->base_component.container, c1->base_component.container);

				if (c1->base_component.container->destroy){
                    destroy_collidable(self, i);
                    break;
                }
				if (c2->base_component.container->destroy){
                    destroy_collidable(self, j);
                    continue;
                }
            }

        }
    }
}

void game_world_set_player(GameWorld *self, Player *p){
    self->player = p;
    int *index = malloc(sizeof(*index));
    //bogus value...deestroy collidable will never be called for player
    *index = 0;
    set_add(self->collidables, &p->collidable);
    set_add(self->indices, index);
}

void game_world_add_object(GameWorld *self, GameObject *object) {
    CollidableComponent *collidable = NULL;
    switch(object->type) {
        case GAME_OBJECT_TYPE_PLAYER:
            collidable = &((Player *)object)->collidable;
            break;
        case GAME_OBJECT_TYPE_ENEMY:
            collidable = &((Enemy *)object)->collidable;
            break;
        case GAME_OBJECT_TYPE_SPELL:
            collidable = &((Spell *)object)->collidable;
            break;
        case GAME_OBJECT_TYPE_WALL:
            collidable = &((Wall *)object)->collidable;
            break;
        default:
            break;
    }
    if (collidable != NULL) {
        int *index = malloc(sizeof(*index));
        *index = set_add(self->game_objects, object);
        set_add(self->collidables, collidable);
        set_add(self->indices, index);
    } else {
        set_add(self->game_objects, object);
    }
}

void game_world_render(GameWorld *self, Mat4 projection_matrix, Mat4 view_matrix){
    renderer_render_sphere(self->renderer, light_position);

    //gather updates to the various things
	Player *p = self->player;
    game_object_render(self->player, self->renderer);

	int i;
	GameObject *obj;
    for(i = 0;i < self->game_objects->length;i++){
		if (self->game_objects->data[i] == NULL) continue;
		obj = self->game_objects->data[i];
        game_object_render(obj, self->renderer);
    }
    level_render(self->level, self->renderer);

    //actually draw stuff
    renderer_render(self->renderer, projection_matrix, view_matrix);
}

int game_world_get_model_id(GameWorld *self, const char *name){
    return renderer_get_model_id(self->renderer, name);
}

Obb game_world_get_model_obb(GameWorld *self, int model_id){
    return renderer_get_model_obb(self->renderer, model_id);
}

// todo change to take a GameObjectType parameter instead of just enemies?
void game_world_apply_to_enemies(GameWorld *self, GameObject *user, float radius, void (*fn)(GameWorld *self, GameObject *user, Enemy *enemy)){
    CollidableComponent *collidable;
    GameObject *object;
    int i;
    for(i = 0;i < self->collidables->length;i++){
        if(self->collidables->data[i] == NULL) continue;
        collidable = self->collidables->data[i];
		object = collidable->base_component.container;
        if(object->type == GAME_OBJECT_TYPE_ENEMY
            && vec3_within_dist(collidable->bounding_box.center, user->position, radius)){
            fn(self, user, (Enemy *)object);
        }
    }
}

Vec3 game_world_world_coords_to_screen_coords(GameWorld *self, Vec3 world_coords){
    Vec3 output;
    float w = mat4_mul_vec3(&output, self->world_to_screen, world_coords);
    output.x /= w;
    output.y /= w;
    output.z /= w;
    return output;
}

Vec3 game_world_screen_coords_to_world_coords(GameWorld *self, Vec3 screen_coords){
    Vec3 direction;
    float w = mat4_mul_vec3(&direction, self->screen_to_world, screen_coords);
    direction.x /= w;
    direction.y /= w;
    direction.z /= w;
    vec3_normalize(&direction);

	Vec3 position = self->camera.location;
    double t = -position.y / direction.y;

    return vec3_add(position, vec3_scale(direction, t));
}

int game_world_is_colliding_with_wall(GameWorld *self, CollidableComponent collidable){
    int i;
    CollidableComponent *other;
    for(i = 0;i < self->collidables->length;i++){
        if(self->collidables->data[i] == NULL) continue;
        other = self->collidables->data[i];
		if (other->base_component.container->type != GAME_OBJECT_TYPE_WALL) continue;
        if(collidable.is_colliding(collidable, *other)
            && other->is_colliding(*other, collidable))
            return 1;
    }
    return 0;
}
