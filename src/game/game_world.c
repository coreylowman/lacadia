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
#include "spell.h"
#include "game_world.h"
#include "collidable_object.h"
#include "game/particle_system.h"
#include "wall.h"

extern Mat4 MAT4_IDENT;
extern Vec3 VEC3_UNIT_Y;

GameWorld *game_world_new(){
    GameWorld *self = malloc(sizeof(*self));
    self->spells = set_new(spell_free);
    self->enemies = set_new(enemy_free);
    self->collidables = set_new(NULL); //these collidables are pointers to other objects collidables... this set doesn't have ownership
    self->indices = set_new(free);
    self->particle_systems = set_new(particle_system_free);

    self->renderer = renderer_new();

    return self;
}

void game_world_free(GameWorld *self){
    set_free(self->spells);
    set_free(self->enemies);
    set_free(self->collidables);
    set_free(self->indices);
    set_free(self->particle_systems);

	renderer_free(self->renderer);

    free(self);
}

static void destroy_collidable(GameWorld *self, int i){
    int index = *(int *)self->indices->data[i];
	CollidableObject *c = self->collidables->data[i];
    switch(c->container->type){
        case GAME_OBJECT_TYPE_SPELL:
            spell_free(self->spells->data[index]);
            self->spells->data[index] = NULL;
            break;
        case GAME_OBJECT_TYPE_ENEMY:
            enemy_free(self->enemies->data[index]);
            self->enemies->data[index] = NULL;
            break;
    }
    set_remove_at(self->collidables, i);
    set_remove_at(self->indices, i);
}

void game_world_update(GameWorld *self, double dt){
    int i, j;
    Spell *s;
    Enemy *e;
    CollidableObject *c1, *c2;

    self->dt = dt;

    player_update(self->player, dt);

    //note: put enemy_update before spell update,
    //so targetted spells capture when their targets are about to be destroyed
    for(i = 0;i < self->enemies->length;i++){
        if(self->enemies->data[i] == NULL) continue;
        e = self->enemies->data[i];
        enemy_update(e, dt);
    }

    for(i = 0;i < self->spells->length;i++){
        if(self->spells->data[i] == NULL) continue;
        s = self->spells->data[i];
        spell_update(s, dt);
    }

    for(i = 0;i < self->collidables->length - 1;i++){
        if(self->collidables->data[i] == NULL) continue;
        c1 = self->collidables->data[i];

        if(c1->container->destroy){
            destroy_collidable(self, i);
            continue;
        }

        for(j = i + 1;j < self->collidables->length;j++){
            if(self->collidables->data[j] == NULL) continue;
            c2 = self->collidables->data[j];
            
            if(c2->container->destroy){
                destroy_collidable(self, j);
                continue;
            }

            if(c1->is_colliding(*c1, *c2) && c2->is_colliding(*c2, *c1)){
                c1->on_collide(c1->container, c2->container);
                c2->on_collide(c2->container, c1->container);

                if(c1->container->destroy){
                    destroy_collidable(self, i);
                    break;
                }
                if(c2->container->destroy){
                    destroy_collidable(self, j);
                    continue;
                }
            }

        }
    }

    ParticleSystem *p;
    for(i = 0;i < self->particle_systems->length;i++){
        if(self->particle_systems->data[i] == NULL) continue;
        p = self->particle_systems->data[i];

        particle_system_update(p, dt);

        if(particle_system_is_over(p)){
            set_remove_at(self->particle_systems, i);
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

void game_world_add_spell(GameWorld *self, void *s){
    Spell *spell = s;
    int *index = malloc(sizeof(*index));
    *index	= set_add(self->spells, s);
    set_add(self->collidables, &spell->collidable);
    set_add(self->indices, index);
}

void game_world_add_enemy(GameWorld *self, void *e){
    Enemy *enemy = e;
    int *index = malloc(sizeof(*index));
    *index = set_add(self->enemies, e);
    set_add(self->collidables, &enemy->collidable);
    set_add(self->indices, index);
}

void game_world_add_wall(GameWorld *self, Wall *w, int i){
    int *index = malloc(sizeof(*index));
    *index = i;
    set_add(self->collidables, &w->collidable);
    set_add(self->indices, index);
}

void game_world_add_particle_system(GameWorld *self, void *ps){
    set_add(self->particle_systems, ps);
}


void game_world_render(GameWorld *self, Mat4 projection_matrix, Mat4 view_matrix){
    int i;
    Spell *s;
	Enemy *e;
    ParticleSystem *ps;
    Vec3 above = vec3_scale(VEC3_UNIT_Y, 1);
    Vec3 healthbar_loc;

    //gather updates to the various things
	Player *p = self->player;
    renderable_object_render(p->renderable, self);
<<<<<<< HEAD
    
=======
    collidable_object_render(p->collidable, self);
>>>>>>> 702ba60... Moving rendering into the renderer object... untested but builds
    for(i = 0;i < self->spells->length;i++){
        if(self->spells->data[i] == NULL) continue;
        s = self->spells->data[i];
        renderable_object_render(s->renderable, self);
        collidable_object_render(s->collidable, self);
    }
	for (i = 0; i < self->enemies->length; i++){
        if(self->enemies->data[i] == NULL) continue;
        e = self->enemies->data[i];
        renderable_object_render(e->renderable, self);
        collidable_object_render(e->collidable, self);
        affectable_object_render(e->affectable, healthbar_loc, self);
    }
    for (i = 0; i < self->particle_systems->length; i++){
        if(self->particle_systems->data[i] == NULL) continue;
		ps = self->particle_systems->data[i];
		particle_system_render(ps);
	}
    level_render(self->level);
<<<<<<< HEAD
	
    for(i = 0;i < self->num_assets;i++){
        ObjectModel *model = self->asset_models[i];
        ArrayList_m4 *model_matrices = self->asset_model_matrices[i];
		
		if (model_matrices->length == 0) continue;

        //upload vertices of model
        glBindBuffer(GL_ARRAY_BUFFER, self->asset_vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, model->num_floats * sizeof(float), &model->vertices[0], GL_STATIC_DRAW);

        //upload colors of model
        glBindBuffer(GL_ARRAY_BUFFER, self->asset_vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, model->num_floats * sizeof(float), &model->colors[0], GL_STATIC_DRAW);

        //upload instace model matrices
        glBindBuffer(GL_ARRAY_BUFFER, self->asset_vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, model_matrices->length * sizeof(Mat4), &model_matrices->data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        //draw stuff
        glBindVertexArray(self->asset_vao);
		glDrawArraysInstanced(GL_TRIANGLES, 0, model->num_floats / 3, model_matrices->length);
        glBindVertexArray(0);

		self->asset_model_matrices[i]->length = 0;
    }
}

void game_world_render_terrain(GameWorld *self, Shader shader){
    level_render_terrain(self->level);
}

void game_world_render_ui(GameWorld *self, Shader shader){
    int i;
    Enemy *e;

    Vec3 above = vec3_scale(VEC3_UNIT_Y, 1);
    Vec3 healthbar_loc;

    healthbar_loc = vec3_add(obb_top(self->player->collidable.bounding_box), above);
    affectable_object_render_ui(self->player->affectable, healthbar_loc, self);

    for (i = 0; i < self->enemies->length; i++){
        if(self->enemies->data[i] == NULL) continue;
        e = self->enemies->data[i];
        healthbar_loc = vec3_add(obb_top(e->collidable.bounding_box), above);
        affectable_object_render_ui(e->affectable, healthbar_loc, self);
    }    

    //upload vertices of model
    glBindBuffer(GL_ARRAY_BUFFER, self->ui_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 8 * self->num_ui_rects * sizeof(float), &self->rects[0], GL_STATIC_DRAW);

    //upload colors of model
    glBindBuffer(GL_ARRAY_BUFFER, self->ui_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 12 * self->num_ui_rects * sizeof(float), &self->rect_colors[0], GL_STATIC_DRAW);

    glBindVertexArray(self->ui_vao);
    glDrawArrays(GL_QUADS, 0, 4 * self->num_ui_rects);
    glBindVertexArray(0);

    self->num_ui_rects = 0;
}

void game_world_debug_render(GameWorld *self, Shader shader){
    int i;
    Spell *s;
    Enemy *e;

    Player *p = self->player;
    collidable_object_render(p->collidable);
    for(i = 0;i < self->spells->length;i++){
        if(self->spells->data[i] == NULL) continue;
        s = self->spells->data[i];
        collidable_object_render(s->collidable);
    }
    for (i = 0; i < self->enemies->length; i++){
        if(self->enemies->data[i] == NULL) continue;
        e = self->enemies->data[i];
        collidable_object_render(e->collidable);
    }
    for(i = 0;i < self->level->num_walls;i++){
        collidable_object_render(self->level->walls[i]->collidable);
    }
}
=======
>>>>>>> 702ba60... Moving rendering into the renderer object... untested but builds

    //actually draw stuff
    renderer_render(self->renderer, projection_matrix, view_matrix);
}

int game_world_get_model_id(GameWorld *self, const char *name){
    return renderer_get_model_id(self->renderer, name);
}

Obb game_world_get_model_obb(GameWorld *self, int model_id){
    return renderer_get_model_obb(self->renderer, model_id);
}

void game_world_apply_to_enemies(GameWorld *self, Vec3 position, float radius, void (*fn)(GameWorld *self, Enemy *enemy)){
    CollidableObject *collidable;
    GameObject *object;
    int i;
    for(i = 0;i < self->collidables->length;i++){
        if(self->collidables->data[i] == NULL) continue;
        collidable = self->collidables->data[i];
        object = collidable->container;
        if(object->type == GAME_OBJECT_TYPE_ENEMY
            && vec3_within_dist(collidable->bounding_box.center, position, radius)){
            fn(self, (Enemy *)object->container);
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

int game_world_is_colliding_with_wall(GameWorld *self, CollidableObject collidable){
    int i;
    CollidableObject *other;
    for(i = 0;i < self->collidables->length;i++){
        if(self->collidables->data[i] == NULL) continue;
        other = self->collidables->data[i];
        if(other->container->type != GAME_OBJECT_TYPE_WALL) continue;
        if(collidable.is_colliding(collidable, *other)
            && other->is_colliding(*other, collidable))
            return 1;
    }
    return 0;
}
