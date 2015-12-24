list(APPEND ENEMIES_SOURCES
    bug.c
    bug.h
    enemy.c
    enemy.h
)

add_library(enemies OBJECT ${ENEMIES_SOURCES})
target_include_directories(enemies
    PUBLIC ${LACADIA_SOURCE_DIR}/src
)