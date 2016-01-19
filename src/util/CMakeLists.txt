list(APPEND UTIL_SOURCES
    array_list.c
    array_list.h
    axis.c
    axis.h
    camera.c
    camera.h
    inputs.c
    inputs.h
    mat4.c
    mat4.h
    object_model.c
    object_model.h
    rect.c
    rect.h
    set.c
    set.h
    shaders.c
    shaders.h
    string_helpers.c
    string_helpers.h
    vec3.c
    vec3.h
    obb.c
    obb.h
)

add_library(util OBJECT ${UTIL_SOURCES})
target_include_directories(util
    PUBLIC ${LACADIA_SOURCE_DIR}/src
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glew/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glfw3/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/lodepng/include
)
