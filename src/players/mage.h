#ifndef MAGE_H
#define MAGE_H

#include "util/game_world.h"
#include "player.h"

Player *mage_new(GameWorld *world);
void mage_on_switch_stance(Player *self);

#endif
