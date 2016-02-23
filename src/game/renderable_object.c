#include "renderable_object.h"

RenderableObject renderable_object_init(const char *model_name, Renderer *renderer){
    RenderableObject self;

    self.model_id = renderer_get_model_id(renderer, model_name);
    mat4_ident(&self.model_matrix);

    return self;
}

void renderable_object_set_model_matrix(RenderableObject *self, Mat4 model_matrix){
    self->model_matrix = model_matrix;
    mat4_transpose(&self->model_matrix);
}

void renderable_object_render(RenderableObject self, Renderer *renderer){
    renderer_render_model(renderer, self.model_id, self.model_matrix);
}

void renderable_object_update(RenderableObject *self, MoveableObject moveable){
    float rotation = moveable_object_get_y_rotation(moveable);
    mat4_ident(&self->model_matrix);
    mat4_rotate_y(&self->model_matrix, rotation);
    mat4_translate(&self->model_matrix, moveable.position);
    mat4_transpose(&self->model_matrix);
}
