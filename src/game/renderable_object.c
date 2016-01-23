#include "renderable_object.h"

void renderable_object_render(RenderableObject self, GameWorld *world){
    game_world_draw_asset(world, self.asset_id, self.model_matrix);
}

void renderable_object_update(RenderableObject *self, MoveableObject moveable){
    float rotation = moveable_object_get_y_rotation(moveable);
    mat4_ident(&self->model_matrix);
    mat4_translate(&self->model_matrix, moveable.position);
    mat4_rotate_y(&self->model_matrix, rotation);
}
