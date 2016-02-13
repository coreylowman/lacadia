#include <stdlib.h>
#include "wall.h"
#include "util/mat4.h"

Wall *wall_new(GameWorld *world, Vec3 position, Vec3 grow_direction, int length){
    Wall *self = malloc(sizeof(*self));
    self->base_object = game_object_new(world, GAME_OBJECT_TYPE_WALL);
    self->base_object->container = self;

	int asset_id = game_world_get_asset_id(world, "assets/wall");

    float width;
    self->collidable.on_collide = wall_on_collide;
    self->collidable.is_colliding = collidable_object_is_colliding;
    self->collidable.container = self->base_object;
    self->collidable.bounding_box = game_world_get_asset_obb(world, asset_id);
    self->collidable.bounding_box.center = position;
    self->collidable.bounding_box.center.y += self->collidable.bounding_box.radius.y;
    width = 2 * self->collidable.bounding_box.radius.x;

    self->num_renderables = length;
    int i;
    Mat4 model_matrix;
    Vec3 pos;
    for(i = 0;i < length;i++){
        pos = vec3_add(position, vec3_scale(grow_direction, width * (float)i));
        mat4_ident(&model_matrix);
        mat4_translate(&model_matrix, pos);
        mat4_transpose(&model_matrix);

        self->renderables[i].asset_id = asset_id;
        self->renderables[i].model_matrix = model_matrix;
    }

    //TODO this only sets up a collidable that contains 1 wall segement...
    //want it to contain length segments
    return self;
}

void wall_free(Wall *self){
    game_object_free(self->base_object);
    free(self);
}

void wall_render(Wall *self){
    int i;
    for(i = 0;i < self->num_renderables;i++){
        renderable_object_render(self->renderables[i], self->base_object->world);        
    }
}

void wall_on_collide(GameObject *self, GameObject *other){
    //do nothing
}

Vec3 wall_dimensions(GameWorld *world){
    int asset_id = game_world_get_asset_id(world, "assets/wall");
    Obb obb = game_world_get_asset_obb(world, asset_id);
    return vec3_scale(obb.radius, 2.0);
}
