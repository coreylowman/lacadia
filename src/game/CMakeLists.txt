list(APPEND GAME_SOURCES    
    affectable_object.c
    affectable_object.h
    collidable_object.c
    collidable_object.h
    colors.h
    game_object.c
    game_object.h
    game_world.c
    game_world.h
    level.c
    level.h
    moveable_object.c
    moveable_object.h
    particle_system.c
    particle_system.h
    renderable_object.c
    renderable_object.h
    stats.h
    wall.c
    wall.h    
)

add_library(game OBJECT ${GAME_SOURCES})
target_include_directories(game
    PUBLIC ${LACADIA_SOURCE_DIR}/src
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glew/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glfw3/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/lodepng/include
)
