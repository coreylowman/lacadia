list(APPEND EFFECTS_SOURCES
    effect.c
    effect.h
    hot.c
    hot.h
    speed.c
    speed.h
)

add_library(effects OBJECT ${EFFECTS_SOURCES})
target_include_directories(effects
    PUBLIC ${LACADIA_SOURCE_DIR}/src
)