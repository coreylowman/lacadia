#ifndef AFFECTABLE_OBJECT_H
#define AFFECTABLE_OBJECT_H

#include "stats.h"
#include "util/set.h"
#include "game/effect.h"
#include "game/game_world.h"
#include "util/mat4.h"

typedef struct AffectableObject {
    Stats stats;
    Set *effects; //Set<Effect *>
} AffectableObject;

//initializes the stats in affectable object
void affectable_object_init(struct AffectableObject *self);

//calls the effects on_apply method
void affectable_object_affect(struct AffectableObject *self, struct Effect *effect);

//updates each of the effects using effects->on_update and manages them
void affectable_object_update(struct AffectableObject *self, double dt);

//renders each of the effects using effects->on_render
void affectable_object_render(struct AffectableObject self, GameWorld *world);
void affectable_object_render_ui(struct AffectableObject self, Mat4 model_matrix, Vec3 pos, GameWorld *world);

void affectable_object_print(struct AffectableObject self);


float affectable_object_damage(struct AffectableObject *self, float amt);

#endif
