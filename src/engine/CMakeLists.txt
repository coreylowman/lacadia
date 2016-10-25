add_subdirectory(util)
add_subdirectory(graphics)

list(APPEND ENGINE_SOURCES
    asset_manager.h
    asset_manager.c
    colors.h
    game_object.c
    game_object.h
    game_world.c
    game_world.h
    particle_system.c
    particle_system.h
    wall.c
    wall.h
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

set(ENGINE_OBJECT_LIBRARIES
    $<TARGET_OBJECTS:util>
    $<TARGET_OBJECTS:graphics>
    $<TARGET_OBJECTS:engine>
)
