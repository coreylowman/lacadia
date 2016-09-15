add_subdirectory(util)

list(APPEND ENGINE_SOURCES
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
    affectable_component.c
    affectable_component.h
    collidable_component.c
    collidable_component.h
    component.c
    component.h
    renderable_component.c
    renderable_component.h
)

add_library(engine OBJECT ${ENGINE_SOURCES})
target_include_directories(engine
    PUBLIC ${LACADIA_INCLUDE_DIRS}
)
