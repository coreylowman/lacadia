#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "player.h"
#include "engine/wall.h"
#include "engine/colors.h"

extern int width, height;

Player *player_new(GameWorld *world, const char *asset_name) {
  Player *self = malloc(sizeof(*self));
  self->base_object = game_object_init(world, "player", player_update,
                                       player_render, player_free);

  self->base_object.position = VEC3_ZERO;
  self->base_object.direction = (Vec3){.data = {0, 0, -1}};

  game_object_alloc_components(&self->base_object, 3);
  self->base_object.components[0] = (Component *)affectable_component_new(
      &self->base_object, 20, 5.0, 0.1, 0, 0);
  self->base_object.components[1] = (Component *)renderable_component_new(
      &self->base_object, asset_name, world->renderer);
  self->base_object.components[2] = (Component *)collidable_component_new(
      &self->base_object,
      game_world_get_model_obb(world, self->renderable->model_id),
      player_on_collide);

  self->affectable = (AffectableComponent *)self->base_object.components[0];
  self->renderable = (RenderableComponent *)self->base_object.components[1];
  self->collidable = (CollidableComponent *)self->base_object.components[2];

  return self;
}

void player_free(GameObject *obj) {
  Player *self = (Player *)obj;
  free(self);
}

void player_update(GameObject *obj, double dt) {
  Player *self = (Player *)obj;
  self->passive(self, dt);

  int i;
  for (i = 0; i < 4; i++) {
    ability_update(&self->abilities[i], dt);
  }

  Inputs inputs = self->base_object.world->inputs;

  if (inputs.tab_pressed) { // move vertically
    printf("switched stance!");
    self->on_switch_stance(self);
  }

  if (inputs.left_mouse_down)
    player_use_ability(self, 0);
  if (inputs.right_mouse_down)
    player_use_ability(self, 1);
  if (inputs.e_pressed)
    player_use_ability(self, 2);
  if (inputs.q_pressed)
    player_use_ability(self, 3);

  double width_2 = (double)(width)*0.5;
  double height_2 = (double)(height)*0.5;
  double mousex = (inputs.mouse_pos[0] - width_2) / width_2;
  double mousey = -(inputs.mouse_pos[1] - height_2) / height_2;
  Vec3 mouse_screen = (Vec3){.data = {mousex, mousey, 1}};
  Vec3 mouse_world = game_world_screen_coords_to_world_coords(
      self->base_object.world, mouse_screen);
  self->base_object.direction =
      vec3_sub(mouse_world, self->base_object.position);
  vec3_normalize(&self->base_object.direction);

  if (inputs.w_down)
    player_move_forwards(self, dt, 1.0);
  if (inputs.s_down)
    player_move_forwards(self, dt, -1.0);
  if (inputs.d_down)
    player_strafe(self, dt, 1.0);
  if (inputs.a_down)
    player_strafe(self, dt, -1.0);
}

void player_render(GameObject *obj, Renderer *renderer) {
  Player *self = (Player *)obj;

  // render the abilities cooldowns
  float spacing = 0.05;
  float width = 0.1;
  float max_height = width * self->base_object.world->camera.aspect_ratio;
  int i;
  Rect2 ability_square;
  ability_square.x = -2 * width - 1.5 * spacing;
  ability_square.y = -0.95;
  ability_square.width = width;
  ability_square.height = max_height;
  for (i = 0; i < 4; i++) {
    ability_square.height =
        max_height *
        (1 - self->abilities[i].cooldown / self->abilities[i].max_cooldown);
    renderer_render_rect(renderer, ability_square, COLOR_RED);
    ability_square.x += width + spacing;
  }
}

void player_use_ability(Player *self, int i) {
  if (ability_is_ready(self->abilities[i])) {
    ability_use(&self->abilities[i], self->base_object.world,
                (GameObject *)self);
  }
}

void player_affect(Player *self, Effect *e, double dt) {
  affectable_component_affect(self->affectable, e);
}

void player_move_forwards(Player *self, double dt, float direction) {
  float speed = self->affectable->speed;
  game_object_move_by(&self->base_object,
                      vec3_scale(VEC3_UNIT_Z, -dt * speed * direction));
}

void player_turn(Player *self, double side_amt) {
  int i;
  Vec3 sideways;

  sideways = vec3_cross(self->base_object.direction, VEC3_UNIT_Y);
  vec3_normalize(&sideways);

  for (i = 0; i < 3; i++)
    self->base_object.direction.data[i] += side_amt * sideways.data[i];

  vec3_normalize(&self->base_object.direction);
}

void player_strafe(Player *self, double dt, float direction) {
  float speed = self->affectable->speed;
  game_object_move_by(&self->base_object,
                      vec3_scale(VEC3_UNIT_X, dt * speed * direction));
}

void player_on_collide(GameObject *self, GameObject *other) {
  Player *player = (Player *)self;
  if (strcmp(other->tag, "wall") == 0) {
    Wall *wall = (Wall *)other;

    Vec3 wall_normal =
        wall_get_normal(wall, player->collidable->bounding_box.center);
    int i;
    for (i = 0; i < 3; i++) {
      player->base_object.position.data[i] +=
          self->world->dt * player->affectable->speed * wall_normal.data[i];
    }
  }
}
