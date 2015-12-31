#include "renderable_object.h"

void renderable_object_render(RenderableObject self, GameWorld *world){
    game_world_draw_asset(world, self.asset_id, self.model_matrix);
}
