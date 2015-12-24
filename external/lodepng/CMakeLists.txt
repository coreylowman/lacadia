list(APPEND LODEPNG_SOURCES
    include/lodepng.h
    src/lodepng.c
)

add_library(lodepng OBJECT ${LODEPNG_SOURCES})
target_include_directories(lodepng
    PUBLIC include
)