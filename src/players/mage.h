#ifndef MAGE_H
#define MAGE_H

#include "game/game_world.h"
#include "player.h"

Player *mage_new(GameWorld *world);
void mage_on_switch_stance(Player *self);
void mage_passive(Player *self, double dt);

#endif
