list(APPEND EFFECTS_SOURCES
    effect.h
    effect.c
    burn.h
    burn.c
    frost.h
    frost.c
)

add_library(effects OBJECT ${EFFECTS_SOURCES})
target_include_directories(effects
    PUBLIC ${LACADIA_SOURCE_DIR}/src
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glew/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glfw3/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/lodepng/include
)
