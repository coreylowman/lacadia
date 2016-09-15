list(APPEND PLAYERS_SOURCES
    player.c
    player.h
    mage.c
    mage.h
)

add_library(players OBJECT ${PLAYERS_SOURCES})
target_include_directories(players
    PUBLIC ${LACADIA_INCLUDE_DIRS}
)