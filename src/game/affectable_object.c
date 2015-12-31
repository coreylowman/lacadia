#include <stdlib.h>
#include "affectable_object.h"

void affectable_object_affect(AffectableObject *self, Effect *effect, double dt){
    effect->on_apply(effect, self, dt);
}

void affectable_object_update(AffectableObject *self, double dt){
    int i;
    Effect *e;
    for(i = 0;i < self->effects->length;i++){
        if(self->effects->data[i] == NULL) continue;
        e = self->effects->data[i];
        e->on_update(e, self, dt);
        if(e->duration <= 0.0)
            e->on_end(e, self, dt);
    }
}
