list(APPEND COMPONENTS_SOURCES
    component.h
    component.c
    affectable_component.h
    affectable_component.c
    collidable_component.h
    collidable_component.c
    renderable_component.h
    renderable_component.c
)

add_library(components OBJECT ${COMPONENTS_SOURCES})
target_include_directories(components
    PUBLIC ${LACADIA_SOURCE_DIR}/src
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glew/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glfw3/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/lodepng/include
)