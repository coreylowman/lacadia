list(APPEND ABILITIES_SOURCES
    ability.c
    ability.h
    combust.c
    combust.h
    fireball.c
    fireball.h
    icicle.c
    icicle.h
    melee_hit.c
    melee_hit.h
    spell.c
    spell.h
    wildfire.c
    wildfire.h
    frost_nova.c
    frost_nova.h
)

add_library(abilities OBJECT ${ABILITIES_SOURCES})
target_include_directories(abilities
    PUBLIC ${LACADIA_SOURCE_DIR}/src
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glew/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glfw3/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/lodepng/include
)
