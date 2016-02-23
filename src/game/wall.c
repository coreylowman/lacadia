#include <stdlib.h>
#include "wall.h"
#include "util/mat4.h"
#include "util/random.h"
#include "util/renderer.h"

extern Vec3 VEC3_UNIT_X;
extern Vec3 VEC3_UNIT_Z;
extern Vec3 VEC3_ZERO;

Wall *wall_new(GameWorld *world, Vec3 position, Vec3 grow_direction, int length){
    Wall *self = malloc(sizeof(*self));
    self->base_object = game_object_new(world, GAME_OBJECT_TYPE_WALL);
    self->base_object->container = self;

	Vec3 dims = wall_dimensions(world);
	int which = grow_direction.x ? 0 : (grow_direction.y ? 1 : 2);
	float width = grow_direction.x ? dims.x : (grow_direction.y ? dims.y : dims.z);

    int model_id = game_world_get_model_id(world, "assets/wall");
    int i;
    Vec3 pos;
    Mat4 model_matrix;
    self->num_renderables = length;
    for(i = 0;i < length;i++){
        pos = vec3_add(position, vec3_scale(grow_direction, width * (float)i));
        mat4_ident(&model_matrix);
		mat4_rotate_y(&model_matrix, 3.14159265358979323846 * 0.5 * (float)random_in_rangei(0, 4));
        mat4_translate(&model_matrix, pos);

<<<<<<< HEAD
        self->renderables[i].asset_id = asset_id;
        renderable_object_set_model_matrix(&self->renderables[i], model_matrix);
    }

    self->collidable.bounding_box = game_world_get_asset_obb(world, asset_id);
=======
		self->renderables[i].model_id = model_id;
        self->renderables[i].model_matrix = model_matrix;
    }

    
    
	self->collidable.bounding_box = game_world_get_model_obb(world, model_id);
>>>>>>> 702ba60... Moving rendering into the renderer object... untested but builds
    self->collidable.bounding_box.center = position;
    self->collidable.bounding_box.center.y += dims.y * 0.5;
    self->collidable.bounding_box.radius.data[which] *= length;
    self->collidable.bounding_box.center.data[which] += width * (length - 1)* 0.5;
    
    self->collidable.container = self->base_object;

    self->collidable.on_collide = wall_on_collide;
    self->collidable.is_colliding = collidable_object_is_colliding;

    self->normal = which == 0 ? VEC3_UNIT_Z : VEC3_UNIT_X;

    return self;
}

void wall_free(Wall *self){
    game_object_free(self->base_object);
    free(self);
}

void wall_render(Wall *self, Renderer *renderer){
    int i;
    for(i = 0;i < self->num_renderables;i++){
        renderable_object_render(self->renderables[i], renderer);
    }
    collidable_object_render(self->collidable, renderer);
}

void wall_on_collide(GameObject *self, GameObject *other){
    //do nothing
}

Vec3 wall_dimensions(GameWorld *world){
    int model_id = game_world_get_model_id(world, "assets/wall");
    Obb obb = game_world_get_model_obb(world, model_id);
    return vec3_scale(obb.radius, 2.0);
}

Vec3 wall_get_normal(Wall *self, Vec3 position){
    int i;
    for(i = 0;i < 3;i++){
        if(self->normal.data[i] == 1.0){
            if(position.data[i] < self->collidable.bounding_box.center.data[i]){
                return vec3_scale(self->normal, -1.0);
            }else{
                return self->normal;
            }
        }
    }
	return VEC3_ZERO;
}
