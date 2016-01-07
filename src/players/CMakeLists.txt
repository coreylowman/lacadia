list(APPEND PLAYERS_SOURCES
    player.c
    player.h
    mage.c
    mage.h
)

add_library(players OBJECT ${PLAYERS_SOURCES})
target_include_directories(players
    PUBLIC ${LACADIA_SOURCE_DIR}/src
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glew/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glfw3/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/lodepng/include
)