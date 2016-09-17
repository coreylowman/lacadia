list(APPEND ABILITIES_SOURCES
    ability.c
    ability.h
    fireball.c
    icicle.c
    spell.c
    spell.h
    melee_hit.c
    # combust.c
    # wildfire.c
    # frost_nova.c
    # arctic_winds.c
    # crystalize.c
    # dragons_breath.c
)

add_library(abilities OBJECT ${ABILITIES_SOURCES})
target_include_directories(abilities
    PUBLIC ${LACADIA_INCLUDE_DIRS}
)
