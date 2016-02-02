#include <stdlib.h>
#include <stdio.h>
#include "affectable_object.h"

void affectable_object_init(AffectableObject *self){
    self->stats.health = self->stats.max_health;
    self->stats.speed = self->stats.max_speed;
    self->stats.regen = self->stats.max_regen;
    self->stats.power = self->stats.max_power;
    self->stats.lifesteal = self->stats.max_lifesteal;
    self->effects = set_new(effect_free);
}

void affectable_object_affect(AffectableObject *self, Effect *effect){
    effect->on_apply(effect, self);
}

void affectable_object_update(AffectableObject *self, double dt){
    int i;
    Effect *e;
    for(i = 0;i < self->effects->length;i++){
        if(self->effects->data[i] == NULL) continue;
        e = self->effects->data[i];

        e->on_update(e, self, dt);
        if(e->is_over(e))
            e->on_end(e, self);
    }
}

void affectable_object_render(AffectableObject self, GameWorld *world){
    int i;
    Effect *e;
    for(i = 0;i < self.effects->length;i++){
        if(self.effects->data[i] == NULL) continue;
        e = self.effects->data[i];

        e->on_render(e, world);
    }
}

void affectable_object_render_ui(AffectableObject self, Mat4 model_matrix, Vec3 world_pos, GameWorld *world){
    Vec3 screen_pos = game_world_world_coords_to_screen_coords(world, model_matrix, world_pos);
	printf("%f %f %f\n", screen_pos.x, screen_pos.y, screen_pos.z);
    Rect2 healthbar;
    healthbar.x = screen_pos.x;
    healthbar.y = screen_pos.y;
    healthbar.width = .1 * self.stats.health / self.stats.max_health;
    healthbar.height = 0.05;
    Vec3 color = (Vec3){ .data = { 1, 0, 0 } };
    game_world_draw_rect(world, healthbar, color);
}

void affectable_object_print(AffectableObject self){
    printf("health: %f %f\n", self.stats.health, self.stats.max_health);
    // printf("speed: %f %f\n", self.stats.speed, self.stats.max_speed);
    // printf("regen: %f %f\n", self.stats.regen, self.stats.max_regen);
    // printf("power: %f %f\n", self.stats.power, self.stats.max_power);
    // printf("lifesteal: %f %f\n", self.stats.lifesteal, self.stats.max_lifesteal);
}

float affectable_object_damage(struct AffectableObject *self, float amt){
    self->stats.health -= amt;
    return amt;
}

