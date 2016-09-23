list(APPEND GRAPHICS_SOURCES
    model_shader.h
    model_shader.c
    line_shader.h
    line_shader.c
    text_shader.h
    text_shader.c
    terrain_shader.h
    terrain_shader.c
    texture_shader.h
    texture_shader.c
    renderer.h
    renderer.c
    shader.h
    shader.c
)

add_library(graphics OBJECT ${GRAPHICS_SOURCES})
target_include_directories(graphics
    PUBLIC ${LACADIA_INCLUDE_DIRS}
)
