#include "renderable_component.h"

void renderable_component_render(Component *self, Renderer *renderer);
void renderable_component_update(Component *self, double dt);

RenderableComponent renderable_component_init(GameObject *container, const char *model_name, Renderer *renderer) {
    RenderableComponent self;

    self.base_component = component_init(container, renderable_component_update, renderable_component_render, NULL);

    self.model_id = renderer_get_model_id(renderer, model_name);
    mat4_ident(&self.model_matrix);

    self.scale = 1.0;

    return self;
}

void renderable_component_set_model_matrix(RenderableComponent *self, Mat4 model_matrix){
    self->model_matrix = model_matrix;
    mat4_transpose(&self->model_matrix);
}

void renderable_component_set_scale(RenderableComponent *self, float scale) {
    self->scale = scale;
}

void renderable_component_render(Component *component, Renderer *renderer){
	RenderableComponent *self = component;
	renderer_render_model(renderer, self->model_id, self->model_matrix);
}

void renderable_component_update(Component *component, double dt){
	RenderableComponent *self = component;
	// this is assuming position is moved to game_object
	GameObject *container = self->base_component.container;
	float rotation = game_object_get_y_rotation(container);
	mat4_ident(&self->model_matrix);
	mat4_rotate_y(&self->model_matrix, rotation);
	mat4_translate(&self->model_matrix, container->position);
    mat4_scale(&self->model_matrix, self->scale);
	mat4_transpose(&self->model_matrix);
}
