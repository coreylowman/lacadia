#ifndef AFFECTABLE_OBJECT_H
#define AFFECTABLE_OBJECT_H

#include "stats.h"
#include "util/set.h"
#include "game/effect.h"

typedef struct AffectableObject {
    Stats stats;
    Set *effects; //Set<Effect *>
} AffectableObject;

void affectable_object_init(struct AffectableObject *self);
void affectable_object_affect(struct AffectableObject *self, struct Effect *effect);
void affectable_object_update(struct AffectableObject *self, double dt);
void affectable_object_print(struct AffectableObject self);

#endif
