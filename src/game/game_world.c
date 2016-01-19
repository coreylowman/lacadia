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

extern Mat4 MAT4_IDENT;

GameWorld *game_world_new(){
    GameWorld *self = malloc(sizeof(*self));
    self->spells = set_new(spell_free);
    self->enemies = set_new(enemy_free);

	self->num_assets = 5;
	self->asset_names[0] = "assets/box";
    self->asset_names[1] = "assets/bug";
    self->asset_names[2] = "assets/fireball";
    self->asset_names[3] = "assets/icicle";
    self->asset_names[4] = "assets/lacadia_mage";
    
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

	int i;
	for (i = 0; i < self->num_assets; i++){
		obj_model_free(self->asset_models[i]);
		array_list_free_m4(self->asset_model_matrices[i]);
	}

    free(self);
}

void game_world_update(GameWorld *self, double dt){
    int i, j;
    Spell *s;
    Enemy *e;

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

    for(i = 0;i < self->spells->length;i++){
        if(self->spells->data[i] == NULL) continue;
        s = self->spells->data[i];
        for(j = 0;j < self->enemies->length;j++){
            if(self->enemies->data[j] == NULL) continue;
            e = self->enemies->data[j];

            if(obb_intersects(s->collidable.bounding_box, e->collidable.bounding_box)){
                s->collidable.on_collide(s->base_object, e->base_object);
                e->collidable.on_collide(e->base_object, s->base_object);
            }
        }

    }
}

void game_world_add_spell(GameWorld *self, void *s){
    set_add(self->spells, s);
}

void game_world_add_enemy(GameWorld *self, void *e){
    set_add(self->enemies, e);
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

int game_world_get_asset_id(GameWorld *self, const char *name){
    int i;
    for(i = 0;i < self->num_assets;i++){
        if(string_equals(self->asset_names[i], name))
            return i;
    }
    return -1;
}

Rect game_world_get_asset_aabb(GameWorld *self, int asset_id){
    return self->asset_models[asset_id]->bounding_box;
}

void game_world_draw_asset(GameWorld *self, int asset_id, Mat4 model_matrix){
    array_list_push_m4(self->asset_model_matrices[asset_id], model_matrix);
}
