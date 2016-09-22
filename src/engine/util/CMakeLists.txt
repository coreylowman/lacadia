list(APPEND UTIL_SOURCES
    array_list.c
    array_list.h
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
    sparse_array.c
    sparse_array.h
    string_helpers.c
    string_helpers.h
    vec3.c
    vec3.h
    line.h
    obb.c
    obb.h
    random.c
    random.h
    simplex_noise.h
    simplex_noise.c
    terrain.h
    terrain.c
)

add_library(util OBJECT ${UTIL_SOURCES})
target_include_directories(util
    PUBLIC ${LACADIA_INCLUDE_DIRS}
)
