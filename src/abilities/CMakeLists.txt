list(APPEND ABILITIES_SOURCES
    ability.c
    ability.h
    dragons_breath.c
    dragons_breath.h
    fireball.c
    fireball.h
    frost_nova.c
    frost_nova.h
    icicle.c
    icicle.h
    spell.c
    spell.h
    teleport.c
    teleport.h
    spread_burn.c
    spread_burn.h
    ice_storm.c
    ice_storm.h
)

add_library(abilities OBJECT ${ABILITIES_SOURCES})
target_include_directories(abilities
    PUBLIC ${LACADIA_SOURCE_DIR}/src
)