list(APPEND COLLECTABLES_SOURCES
    collectable.h
    collectable.c
    coin.h
    coin.c
)

add_library(collectables OBJECT ${COLLECTABLES_SOURCES})
target_include_directories(collectables
    PUBLIC ${LACADIA_INCLUDE_DIRS}
)