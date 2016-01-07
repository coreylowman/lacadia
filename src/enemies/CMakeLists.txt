list(APPEND ENEMIES_SOURCES
    bug.c
    bug.h
    enemy.c
    enemy.h
)

add_library(enemies OBJECT ${ENEMIES_SOURCES})
target_include_directories(enemies
    PUBLIC ${LACADIA_SOURCE_DIR}/src
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glew/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glfw3/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/lodepng/include
)