list(APPEND GAME_SOURCES
    ability.c
    ability.h
    affectable_object.c
    affectable_object.h
    collidable_object.h
    effect.c
    effect.h
    game_object.c
    game_object.h
    game_world.c
    game_world.h
    moveable_object.c
    moveable_object.h
    renderable_object.c
    renderable_object.h
    spell.c
    spell.h
    stats.h
    wall.h
    wall.c
)

add_library(game OBJECT ${GAME_SOURCES})
target_include_directories(game
    PUBLIC ${LACADIA_SOURCE_DIR}/src
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glew/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glfw3/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/lodepng/include
)
