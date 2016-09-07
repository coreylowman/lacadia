list(APPEND COLLECTABLES_SOURCES
    collectable.h
    collectable.c
    coin.h
    coin.c
)

add_library(collectables OBJECT ${COLLECTABLES_SOURCES})
target_include_directories(collectables
    PUBLIC ${LACADIA_SOURCE_DIR}/src
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glew/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/glfw3/include
    PUBLIC ${LACADIA_SOURCE_DIR}/external/lodepng/include
)