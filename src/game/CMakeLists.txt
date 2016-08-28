list(APPEND GAME_SOURCES
    colors.h
    game_object.c
    game_object.h
    game_world.c
    game_world.h
    level.c
    level.h
    particle_system.c
    particle_system.h
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
