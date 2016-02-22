list(APPEND GAME_SOURCES
    ability.c
    ability.h
    affectable_object.c
    affectable_object.h
    burn.c
    burn.h
    collidable_object.c
    collidable_object.h
    colors.h
    combust.c
    combust.h
    effect.c
    effect.h
    fireball.c
    fireball.h
    game_object.c
    game_object.h
    game_world.c
    game_world.h
    level.c
    level.h
    melee_hit.c
    melee_hit.h
    moveable_object.c
    moveable_object.h
    particle_system.c
    particle_system.h
    renderable_object.c
    renderable_object.h
    spell.c
    spell.h
    stats.h
    wall.c
    wall.h
    wildfire.c
    wildfire.h
)

add_library(game OBJECT ${GAME_SOURCES})
target_include_directories(game
    PUBLIC ${LACADIA_SOURCE_DIR}/src
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glew/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glfw3/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/lodepng/include
)
