#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "game_world.h"
#include "array_list.h"
#include "abilities/spell.h"
#include "players/player.h"
#include "enemies/enemy.h"
#include "rect.h"
#include "game_object.h"
#include "string_helpers.h"

extern Mat4 MAT4_IDENT;

GameWorld *game_world_new(){
    GameWorld *self = malloc(sizeof(*self));
    self->spells = array_list_new(spell_free);
    self->enemies = array_list_new(enemy_free);

	self->num_assets = 3;
	self->asset_names[0] = "assets/lacadia_mage";
    self->asset_names[1] = "assets/fireball";
	self->asset_names[2] = "assets/box";

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
    array_list_free(self->spells);
    array_list_free(self->enemies);

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
        s = self->spells->data[i];
		spell_update(s, dt);
    }
    for(i = 0;i < self->enemies->length;i++){
        e = self->enemies->data[i];
        enemy_update(e, dt);
    }

    for(i = 0;i < self->spells->length;i++){
        s = self->spells->data[i];
        for(j = 0;j < self->enemies->length;j++){
            e = self->enemies->data[j];
            if(rect_intersects(s->base_object->bounding_box, e->base_object->bounding_box)){
				s->on_collide(s, e->base_object, ((Player *)self->player)->base_object);
				if (s->destroy_on_collide){
					s->base_object->destroy = 1;
				}
            }
        }
    }
	for (i = self->spells->length - 1; i >= 0; i--){
		s = self->spells->data[i];
		if (s->base_object->destroy)
			array_list_remove_at(self->spells, i);
	}
}

void game_world_render(GameWorld *self, Shader shader){
    int i;
    Spell *s;
	Enemy *e;

    //gather updates to the vertices
    game_object_render(self->player->base_object);
    for(i = 0;i < self->spells->length;i++){
        s = self->spells->data[i];
        game_object_render(s->base_object);
    }
	for (i = 0; i < self->enemies->length; i++){
		e = self->enemies->data[i];
		game_object_render(e->base_object);
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
