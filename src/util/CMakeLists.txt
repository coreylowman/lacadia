list(APPEND UTIL_SOURCES
    array_list.c
    array_list.h
    axis.c
    axis.h
    camera.c
    camera.h
    game_object.c
    game_object.h
    game_world.c
    game_world.h
    inputs.c
    inputs.h
    mat4.c
    mat4.h
    rect.c
    rect.h
    log.c
    log.h
    string_helpers.c
    string_helpers.h
    shaders.c
    shaders.h
    vec3.c
    vec3.h
    object_model.c
    object_model.h
)

add_library(util OBJECT ${UTIL_SOURCES})
target_include_directories(util
    PUBLIC ${LACADIA_SOURCE_DIR}/src
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glew/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glfw3/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/lodepng/include
)
