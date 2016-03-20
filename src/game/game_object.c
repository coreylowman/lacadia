#include <string.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include "game_object.h"

GameObject game_object_init(GameWorld *world, GameObjectType type){
    GameObject self;

    self.type = type;

    self.world = world;

    self.destroy = 0;

    return self;   
}
