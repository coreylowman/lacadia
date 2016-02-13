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
#include "game/particle_system.h"

extern Mat4 MAT4_IDENT;
extern Vec3 VEC3_UNIT_Y;

GameWorld *game_world_new(){
    GameWorld *self = malloc(sizeof(*self));
    self->spells = set_new(spell_free);
    self->enemies = set_new(enemy_free);
    self->collidables = set_new(NULL); //these collidables are pointers to other objects collidables... this set doesn't have ownership
    self->indices = set_new(free);
    self->particle_systems = set_new(particle_system_free);

    printf("Loading assets... ");
	self->num_assets = 8;
	self->asset_names[0] = "assets/default_box";
    self->asset_names[1] = "assets/mage";
	self->asset_names[2] = "assets/hunter";
	self->asset_names[3] = "assets/bug";
    self->asset_names[4] = "assets/fireball";
    self->asset_names[5] = "assets/icicle";
    self->asset_names[6] = "assets/burn_particle";
    self->asset_names[7] = "assets/wall";
    
    int i;
    for(i = 0;i < self->num_assets;i++){
        self->asset_models[i] = obj_model_from_file(self->asset_names[i]);        
		self->asset_model_matrices[i] = array_list_new_m4();
    }
    printf("done.\n");

    glGenVertexArrays(1, &self->asset_vao);
    glBindVertexArray(self->asset_vao);
	
    glGenBuffers(3, &self->asset_vbo[0]);
    
    //vertex data buffer object
	glBindBuffer(GL_ARRAY_BUFFER, self->asset_vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    //color data buffer object
	glBindBuffer(GL_ARRAY_BUFFER, self->asset_vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, self->asset_vbo[2]);
    //instance model matrix buffer object
    for(i = 0;i < 4;i++){
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void *)(4 * i * sizeof(float)));
        glEnableVertexAttribArray(2 + i);
        glVertexAttribDivisor(2 + i, 1);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //ui vbo/vao
    glGenVertexArrays(1, &self->ui_vao);
    glBindVertexArray(self->ui_vao);
    
    glGenBuffers(2, &self->ui_vbo[0]);
    
    //vertex data buffer object
    glBindBuffer(GL_ARRAY_BUFFER, self->ui_vbo[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    //color data buffer object
    glBindBuffer(GL_ARRAY_BUFFER, self->ui_vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
	self->num_ui_rects = 0;

    return self;
}

void game_world_free(GameWorld *self){
    set_free(self->spells);
    set_free(self->enemies);
    set_free(self->collidables);
    set_free(self->indices);
    set_free(self->particle_systems);

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

void game_world_add_particle_system(GameWorld *self, void *ps){
    set_add(self->particle_systems, ps);
}

void game_world_render(GameWorld *self, Shader shader){
    int i;
    Spell *s;
	Enemy *e;
    ParticleSystem *ps;

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
    for (i = 0; i < self->particle_systems->length; i++){
        if(self->particle_systems->data[i] == NULL) continue;
		ps = self->particle_systems->data[i];
		particle_system_render(ps);
	}
	
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

void game_world_render_ui(GameWorld *self, Shader shader){
    int i;
    Enemy *e;
    Vec3 above = vec3_scale(VEC3_UNIT_Y, 1);
    Vec3 healthbar_loc;
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
}

int game_world_get_asset_id(GameWorld *self, const char *name){
    int i;
    for(i = 0;i < self->num_assets;i++){
        if(string_equals(self->asset_names[i], name))
            return i;
    }
	//default to default box
    return 0;
}

Obb game_world_get_asset_obb(GameWorld *self, int asset_id){
    return self->asset_models[asset_id]->bounding_box;
}

void game_world_draw_asset(GameWorld *self, int asset_id, Mat4 model_matrix){
    array_list_push_m4(self->asset_model_matrices[asset_id], model_matrix);
}

void game_world_draw_rect(GameWorld *self, Rect2 rect, Vec3 color){
    int i = self->num_ui_rects;
    if(i < MAX_UI_RECTS){
        self->rects[i] = ui_rect_from_rect2(rect);
        self->rect_colors[4*i] = color;
        self->rect_colors[4*i+1] = color;
        self->rect_colors[4*i+2] = color;
        self->rect_colors[4*i+3] = color;
        self->num_ui_rects++;
    }else{
        printf("error! too many rects\n");
    }
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
