list(APPEND EFFECTS_SOURCES
    affectable_component.c
    affectable_component.h
    effect.h
    effect.c
    burn.h
    burn.c
    frost.h
    frost.c
    permafrost.h
    permafrost.c
    soulburn.h
    soulburn.c
)

add_library(effects OBJECT ${EFFECTS_SOURCES})
target_include_directories(effects
    PUBLIC ${LACADIA_INCLUDE_DIRS}
)
