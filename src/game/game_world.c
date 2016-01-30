#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

#include "players/player.h"
#include "enemies/enemy.h"
#include "util/array_list.h"
#include "util/obb.h"
#include "util/string_helpers.h"
#include "game_object.h"
#include "spell.h"
#include "game_world.h"
#include "collidable_object.h"

extern Mat4 MAT4_IDENT;

static void null_free(void *data){

}

GameWorld *game_world_new(){
    GameWorld *self = malloc(sizeof(*self));
    self->spells = set_new(spell_free);
    self->enemies = set_new(enemy_free);
    self->collidables = set_new(null_free); //these collidables are pointers to other objects collidables... this set doesn't have ownership
    self->indices = set_new(free);

	self->num_assets = 6;
	self->asset_names[0] = "assets/box";
    self->asset_names[1] = "assets/bug";
    self->asset_names[2] = "assets/fireball";
    self->asset_names[3] = "assets/icicle";
    self->asset_names[4] = "assets/lacadia_mage";
    self->asset_names[5] = "assets/burn_particle";
    
    int i;
    for(i = 0;i < self->num_assets;i++){
        self->asset_models[i] = obj_model_from_file(self->asset_names[i]);        
        self->asset_model_matrices[i] = array_list_new_m4();
    }

    glGenVertexArrays(1, &self->vao);
    glBindVertexArray(self->vao);
	
    glGenBuffers(3, &self->vbo[0]);
    
    //vertex data buffer object
	glBindBuffer(GL_ARRAY_BUFFER, self->vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    //color data buffer object
	glBindBuffer(GL_ARRAY_BUFFER, self->vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, self->vbo[2]);
    //instance model matrix buffer object
    for(i = 0;i < 4;i++){
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void *)(4 * i * sizeof(float)));
        glEnableVertexAttribArray(2 + i);
        glVertexAttribDivisor(2 + i, 1);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return self;
}

void game_world_free(GameWorld *self){
    set_free(self->spells);
    set_free(self->enemies);
    set_free(self->collidables);
    set_free(self->indices);

	int i;
	for (i = 0; i < self->num_assets; i++){
		obj_model_free(self->asset_models[i]);
		array_list_free_m4(self->asset_model_matrices[i]);
	}

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

    player_update(self->player, dt);

    for(i = 0;i < self->spells->length;i++){
        if(self->spells->data[i] == NULL) continue;
        s = self->spells->data[i];
        spell_update(s, dt);
    }

    for(i = 0;i < self->enemies->length;i++){
        if(self->enemies->data[i] == NULL) continue;
        e = self->enemies->data[i];
        enemy_update(e, dt);
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

void game_world_render(GameWorld *self, Shader shader){
    int i;
    Spell *s;
	Enemy *e;

    //gather updates to the vertices
	Player *p = self->player;
    renderable_object_render(p->renderable, self);
    for(i = 0;i < self->spells->length;i++){
        if(self->spells->data[i] == NULL) continue;
        s = self->spells->data[i];
        renderable_object_render(s->renderable, self);
    }
	for (i = 0; i < self->enemies->length; i++){
        if(self->enemies->data[i] == NULL) continue;
		e = self->enemies->data[i];
		renderable_object_render(e->renderable, self);
        affectable_object_render(e->affectable, self);
	}
	
    for(i = 0;i < self->num_assets;i++){
        ObjectModel *model = self->asset_models[i];
        ArrayList_m4 *model_matrices = self->asset_model_matrices[i];
		
		if (model_matrices->length == 0) continue;

        //upload vertices of model
        glBindBuffer(GL_ARRAY_BUFFER, self->vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, model->num_floats * sizeof(float), &model->vertices[0], GL_STATIC_DRAW);

        //upload colors of model
        glBindBuffer(GL_ARRAY_BUFFER, self->vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, model->num_floats * sizeof(float), &model->colors[0], GL_STATIC_DRAW);

        //upload instace model matrices
        glBindBuffer(GL_ARRAY_BUFFER, self->vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, model_matrices->length * sizeof(Mat4), &model_matrices->data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        //draw stuff
        glBindVertexArray(self->vao);
		glDrawArraysInstanced(GL_TRIANGLES, 0, model->num_floats, model_matrices->length);
        glBindVertexArray(0);

		self->asset_model_matrices[i]->length = 0;
    }
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
}

int game_world_get_asset_id(GameWorld *self, const char *name){
    int i;
    for(i = 0;i < self->num_assets;i++){
        if(string_equals(self->asset_names[i], name))
            return i;
    }
    return -1;
}

Obb game_world_get_asset_obb(GameWorld *self, int asset_id){
    return self->asset_models[asset_id]->bounding_box;
}

void game_world_draw_asset(GameWorld *self, int asset_id, Mat4 model_matrix){
    array_list_push_m4(self->asset_model_matrices[asset_id], model_matrix);
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
