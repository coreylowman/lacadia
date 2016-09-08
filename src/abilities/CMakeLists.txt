list(APPEND ABILITIES_SOURCES
    ability.c
    ability.h
    combust.c
    fireball.c
    icicle.c
    melee_hit.c
    spell.c
    spell.h
    wildfire.c
    frost_nova.c
    arctic_winds.c
    crystalize.c
    dragons_breath.c
)

add_library(abilities OBJECT ${ABILITIES_SOURCES})
target_include_directories(abilities
    PUBLIC ${LACADIA_INCLUDE_DIRS}
)
