#include <stdlib.h>
#include <stdio.h>
#include "affectable_component.h"
#include "game/game_world.h"
#include "util/obb.h"
#include "game/colors.h"

void affectable_component_update(Component *self, double dt);
void affectable_component_render(Component *self, Renderer *renderer);
void affectable_component_free(void *self);

AffectableComponent affectable_component_init(GameObject *container, float max_health, float max_speed, float max_regen, float max_power, float max_lifesteal){
    AffectableComponent self;

    self.base_component = component_init(container, affectable_component_update, affectable_component_render, affectable_component_free);

    self.max_health = max_health;
    self.max_speed = max_speed;
    self.max_regen = max_regen;
    self.max_power = max_power;
    self.max_lifesteal = max_lifesteal;

    self.health = self.max_health;
    self.speed = self.max_speed;
    self.regen = self.max_regen;
    self.power = self.max_power;
    self.lifesteal = self.max_lifesteal;

	int i;
	for (i = 0; i < EFFECT_TYPE_MAX; i++){
		self.effects[i] = NULL;
	}

    return self;
}

void affectable_component_affect(AffectableComponent *self, Effect *effect){
    effect->on_apply(effect, self);
}

void affectable_component_update(Component *component, double dt){
	AffectableComponent *self = component;
    int i;
    Effect *e;
    for(i = 0;i < EFFECT_TYPE_MAX;i++){
        if(self->effects[i] == NULL) continue;
        e = self->effects[i];

        if(e->is_over(e))
            e->on_end(e, self);
        else
            e->on_update(e, self, dt);
    }
}

void affectable_component_render(Component *component, Renderer *renderer){
	AffectableComponent *self = component;
    int i;
    Effect *e;
    for(i = 0;i < EFFECT_TYPE_MAX;i++){
        if(self->effects[i] == NULL) continue;
        e = self->effects[i];

        e->on_render(e, renderer);
    }

    GameObject *container = self->base_component.container;

    // this is assuming position is moved to GameObject
    Vec3 screen_pos = vec3_add(container->position, vec3_scale(VEC3_UNIT_Y, 5));
    screen_pos = game_world_world_coords_to_screen_coords(container->world, screen_pos);
    
    Rect2 healthbar;
    healthbar.x = screen_pos.x - 0.05;
    healthbar.y = screen_pos.y;
    healthbar.width = .1 * self->health / self->max_health;
    healthbar.height = 0.01;
    renderer_render_rect(renderer, healthbar, COLOR_RED);

    Rect2 background;
    background.x = screen_pos.x - 0.05;
    background.y = screen_pos.y;
    background.width = .1;
    background.height = 0.01;
    renderer_render_rect(renderer, background, COLOR_WHITE);
}

void affectable_component_free(void *component) {
    AffectableComponent *self = component;
    int i;
    for(i = 0;i < EFFECT_TYPE_MAX;i++) {
		if (self->effects[i] != NULL){
			effect_free(self->effects[i]);
		}
    }
}

float affectable_component_damage(AffectableComponent *self, float amt){
    self->health -= amt;
    return amt;
}

float affectable_component_slow(AffectableComponent *self, float amt) {
    self->speed -= amt;
    return amt;
}

float affectable_component_quicken(AffectableComponent *self, float amt) {
    self->speed += amt;
    return amt;
}
